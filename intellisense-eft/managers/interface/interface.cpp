#include "interface.hpp"

#include <managers/managers.hpp>

LRESULT __stdcall input_wndproc(HWND hwnd, uint32_t msg, WPARAM wp, LPARAM lp) {
	auto& if_mgr = managers::mgrs.interface_mgr;

	if (if_mgr.render_interface) {
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wp, lp)) {
			return 1;
		}
	}

	switch (msg)
	{
		case WM_KEYDOWN:
			if (wp == VK_INSERT)
				if_mgr.render_interface = !if_mgr.render_interface;
			break;
	}

	return ::CallWindowProcW(if_mgr.original_wnd_proc, hwnd, msg, wp, lp);
}

HRESULT __stdcall present_hook( IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags )
{
	auto& if_mgr = managers::mgrs.interface_mgr;

	std::call_once( if_mgr.is_init, [ & ] ( )
		{
			swap_chain->GetDesc( &if_mgr.sd );
			swap_chain->GetDevice( __uuidof( ID3D11Device ), reinterpret_cast< void** >( &if_mgr.global_device ) );
			swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &if_mgr.back_buffer ) );

			if_mgr.window = if_mgr.sd.OutputWindow;
			if_mgr.original_wnd_proc = (WNDPROC)SetWindowLongPtrW(if_mgr.window, GWLP_WNDPROC, (LONG_PTR)&input_wndproc);

			if_mgr.global_device->GetImmediateContext( &if_mgr.global_context );
			if_mgr.global_device->CreateRenderTargetView( if_mgr.back_buffer, nullptr, &if_mgr.main_render_target_view ); if_mgr.back_buffer->Release( );

			ImGui::CreateContext( );
			ImGuiIO& io = ImGui::GetIO( ); ( void )io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

			managers::mgrs.menu_mgr.set_style( );

			ImGuiStyle& style = ImGui::GetStyle( );
			if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
			{
				style.WindowRounding = 0.0f;
				style.Colors [ ImGuiCol_WindowBg ].w = 1.0f;
			}

			ImGui_ImplWin32_Init( if_mgr.window );
			ImGui_ImplDX11_Init( if_mgr.global_device, if_mgr.global_context );
		} );

	if ( if_mgr.main_render_target_view == nullptr )
	{
		swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &if_mgr.back_buffer ) );
		if_mgr.global_device->CreateRenderTargetView( if_mgr.back_buffer, nullptr, &if_mgr.main_render_target_view );

		if_mgr.back_buffer->Release( );
	}

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );

	ImGui::NewFrame( );

	const auto bg_dl = ImGui::GetBackgroundDrawList();
	managers::mgrs.interface_mgr.m_draw.begin();
	managers::mgrs.interface_mgr.m_draw.draw(bg_dl);
	managers::mgrs.interface_mgr.m_draw.clear();
	managers::mgrs.interface_mgr.m_draw.end();

	if ( if_mgr.render_interface )
		managers::mgrs.menu_mgr.render( );

	ImGui::Render( );

	if_mgr.global_context->OMSetRenderTargets( 1, &if_mgr.main_render_target_view, NULL );
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

	ImGuiIO& io = ImGui::GetIO( );
	if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
	{
		ImGui::UpdatePlatformWindows( );
		ImGui::RenderPlatformWindowsDefault( );
	}

	return if_mgr.d3d11_present( swap_chain, sync_interval, flags );
}

HRESULT __stdcall resize_buffers_hook( IDXGISwapChain* this_ptr, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags )
{
	auto& if_mgr = managers::mgrs.interface_mgr;

	if ( if_mgr.main_render_target_view )
	{
		if_mgr.main_render_target_view->Release( );
		if_mgr.main_render_target_view = nullptr;
	}

	if_mgr.window = if_mgr.sd.OutputWindow;

	GetWindowRect( if_mgr.global_hwnd, &if_mgr.rc );
	GetClientRect( if_mgr.global_hwnd, &if_mgr.client_rect );

	if_mgr.s_w = if_mgr.rc.right - if_mgr.rc.left;
	if_mgr.s_h = if_mgr.rc.bottom - if_mgr.rc.top;

	return if_mgr.d3d11_resize_buffers( this_ptr, buffer_count, width, height, new_format, swap_chain_flags );
}


void managers::interface_manager_t::initiate( )
{
	global_raw_hwnd = FindWindowW( 0, L"EscapeFromTarkov" );
	global_hwnd = reinterpret_cast< HWND >( CreateMenu( ) );

	SetForegroundWindow( global_raw_hwnd );
	GetWindowRect( global_raw_hwnd, &rc );
	GetClientRect( global_raw_hwnd, &client_rect );

	s_w = rc.right - rc.left;
	s_h = rc.bottom - rc.top;

	D3D_FEATURE_LEVEL levels [ ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtained_level;

	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 2;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow = global_hwnd;
		sd.SampleDesc.Count = 1;
		sd.Windowed = ( ( GetWindowLongPtrA( global_hwnd, GWL_STYLE ) & WS_POPUP ) != 0 ) ? false : true;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.BufferDesc.Width = 1;
		sd.BufferDesc.Height = 1;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}

	D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, sizeof( levels ) / sizeof( D3D_FEATURE_LEVEL ), D3D11_SDK_VERSION, &sd, &global_swapchain, &global_device, &obtained_level, &global_context );

	std::uintptr_t* vt_swapchain;
	std::memcpy( &vt_swapchain, reinterpret_cast< LPVOID >( global_swapchain ), sizeof( std::uintptr_t ) );

	DWORD old_protection;
	VirtualProtect( vt_swapchain, sizeof( std::uintptr_t ), PAGE_EXECUTE_READWRITE, &old_protection );

	d3d11_present = reinterpret_cast< decltype( d3d11_present ) >( vt_swapchain [ 8 ] );
	d3d11_resize_buffers = reinterpret_cast< decltype( d3d11_resize_buffers ) >( vt_swapchain [ 13 ] );

	vt_swapchain [ 8 ] = reinterpret_cast< std::uintptr_t >( &present_hook );
	vt_swapchain [ 13 ] = reinterpret_cast< std::uintptr_t >( &resize_buffers_hook );

	VirtualProtect( vt_swapchain, sizeof( std::uintptr_t ), old_protection, &old_protection );
}