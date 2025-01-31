#include "stdafx.h"
#include "GraphicsEngine.h"

Light g_light;								//ライト。

GraphicsEngine::~GraphicsEngine()
{
	WaitDraw();
	//後始末。
	if (m_commandQueue) {
		m_commandQueue->Release();
	}
	if (m_swapChain) {
		m_swapChain->Release();
	}
	if (m_rtvHeap) {
		m_rtvHeap->Release();
	}
	if (m_dsvHeap) {
		m_dsvHeap->Release();
	}
	if (m_commandAllocator) {
		m_commandAllocator->Release();
	}
	if (m_commandList) {
		m_commandList->Release();
	}
	if (m_pipelineState) {
		m_pipelineState->Release();
	}
	for (auto& rt : m_renderTargets) {
		if (rt) {
			rt->Release();
		}
	}
	if (m_depthStencilBuffer) {
		m_depthStencilBuffer->Release();
	}
	if (m_fence) {
		m_fence->Release();
	}

	if (m_d3dDevice) {
		m_d3dDevice->Release();
	}

	CloseHandle(m_fenceEvent);
}
void GraphicsEngine::WaitDraw()
{
	//描画終了待ち
	// Signal and increment the fence value.
	const UINT64 fence = m_fenceValue;
	m_commandQueue->Signal(m_fence, fence);
	m_fenceValue++;

	// Wait until the previous frame is finished.
	if (m_fence->GetCompletedValue() < fence)
	{
		m_fence->SetEventOnCompletion(fence, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}
bool GraphicsEngine::Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	m_frameBufferWidth = frameBufferWidth;
	m_frameBufferHeight = frameBufferHeight;

	//デバイスにアクセスするためのインターフェースを作成。
	auto dxgiFactory = CreateDXGIFactory();
	//D3Dデバイスの作成。
	if (!CreateD3DDevice( dxgiFactory ) ) {
		//D3Dデバイスの作成に失敗した。
		MessageBox(hwnd, TEXT("D3Dデバイスの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}
	//コマンドキューの作成。
	if (!CreateCommandQueue()) {
		//コマンドキューの作成に失敗した。
		MessageBox(hwnd, TEXT("コマンドキューの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}
	//スワップチェインを作成。
	if (!CreateSwapChain(hwnd, frameBufferWidth, frameBufferHeight, dxgiFactory)) {
		//スワップチェインの作成に失敗。
		MessageBox(hwnd, TEXT("スワップチェインの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}
	
	//フレームバッファ用のディスクリプタヒープを作成する。
	if (!CreateDescriptorHeapForFrameBuffer()) {
		MessageBox(hwnd, TEXT("フレームバッファ用のディスクリプタヒープの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}

	//フレームバッファ用のRTVを作成する。
	if (!CreateRTVForFameBuffer()) {
		MessageBox(hwnd, TEXT("フレームバッファ用のRTVの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;

	}

	//フレームバッファ用のDSVを作成する。
	if (!CreateDSVForFrameBuffer(frameBufferWidth, frameBufferHeight)) {
		MessageBox(hwnd, TEXT("フレームバッファ用のDSVの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}

	//コマンドアロケータの作成。
	m_d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_commandAllocator));

	if (!m_commandAllocator) {
		MessageBox(hwnd, TEXT("コマンドアロケータの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}

	//コマンドリストの作成。
	if (!CreateCommandList()) {
		MessageBox(hwnd, TEXT("コマンドリストの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}

	//GPUと同期をとるためのオブジェクトを作成する。
	if (!CreateSynchronizationWithGPUObject()) {
		MessageBox(hwnd, TEXT("GPUと同期をとるためのオブジェクトの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}
#ifdef MODE_DEBUG
	if (!CreateDebug()) {
		MessageBox(hwnd, TEXT("デバッグレイヤーの作成に失敗。"), TEXT("エラー。"), MB_OK);
	}
#endif 


	
	//レンダリングコンテキストの作成。
	m_renderContext.Init(m_commandList);

	//ビューポートを初期化。
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<FLOAT>(frameBufferWidth);
	m_viewport.Height = static_cast<FLOAT>(frameBufferHeight);
	m_viewport.MinDepth = D3D12_MIN_DEPTH;
	m_viewport.MaxDepth = D3D12_MAX_DEPTH;

	//シザリング矩形を初期化。
	m_scissorRect.left = 0;
	m_scissorRect.top = 0;
	m_scissorRect.right = static_cast<LONG>(5000.0f);
	m_scissorRect.bottom = static_cast<LONG>(5000.0f);

	//CBR_SVRのディスクリプタのサイズを取得。
	m_cbrSrvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//初期化が終わったのでDXGIを破棄。
	dxgiFactory->Release();

	//カメラを初期化する。
	m_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	m_camera2D.SetWidth( static_cast<float>(m_frameBufferWidth) );
	m_camera2D.SetHeight( static_cast<float>(m_frameBufferHeight) );
	m_camera2D.SetPosition({0.0f, 0.0f, 1.0f});
	m_camera2D.SetTarget({ 0.0f, 0.0f, 0.0f });

	m_camera3D.SetPosition({0.0f, 50.0f, 200.0f} );
	m_camera3D.SetTarget({ 0.0f, 0.0f, 0.0f });

	//ライトの設定。
	/*g_light.directionalLight[3].color = { 0.2f, 0.2f, 0.2f, 1.0f };
	g_light.directionalLight[3].direction = { -1.0f, -1.0f, -1.0f};
	g_light.directionalLight[3].direction.Normalize();

	g_light.directionalLight[1].color = { 0.5f, 0.5f, 0.5f, 1.0f };
	g_light.directionalLight[1].direction = { 1.0f, 1.0f, -1.0f };
	g_light.directionalLight[1].direction.Normalize();

	g_light.directionalLight[2].color = { 0.4, 0.4, 0.4, 1.0f };
	g_light.directionalLight[2].direction = { 0.0f, 0.0f, -1.0f };*/
	
	g_light.directionalLight[0].color = { 1.4f, 1.4f, 1.4f, 1.0f };
	g_light.directionalLight[0].direction = { 1.0f, -1.0f, 0.3f };
	g_light.directionalLight[0].direction.Normalize();
	//環境光。
	g_light.ambinetLight = { 0.5f, 0.5f, 0.5f };

	g_light.eyePos = GraphicsEngineObj()->GetCamera3D().GetPosition();
	g_light.specPow = 5.0f;

	//DirectXTK用のグラフィックメモリ管理クラスのインスタンスを作成する。
	m_directXTKGfxMemroy = std::make_unique<DirectX::GraphicsMemory>(m_d3dDevice);

	m_renderingEngine.Init();

	//フォントエンジン初期化。
	m_fontEngine.Init(m_renderContext);

	//GBufferを初期化。
	//todo:preRenderClass
	m_defferd.Init();

	//シャドウマップのインスタンス。
	m_shadow = new DirectionalShadowMap;
	m_shadow->Init(5000, 5000, 3000.0f);

	m_lineDraw.Init();

	return true;
}
IDXGIFactory4* GraphicsEngine::CreateDXGIFactory()
{
	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
	//デバッグコントローラーがあれば、デバッグレイヤーがあるDXGIを作成する。
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();

		// Enable additional debug layers.
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		debugController->Release();
	}
#endif
	IDXGIFactory4* factory;
	CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
	return factory;
}

bool GraphicsEngine::CreateD3DDevice( IDXGIFactory4* dxgiFactory )
{
	D3D_FEATURE_LEVEL fuatureLevel[] = {
		D3D_FEATURE_LEVEL_12_1,	//Direct3D 12.1の機能を使う。
		D3D_FEATURE_LEVEL_12_0	//Direct3D 12.0の機能を使う。
	};

	IDXGIAdapter* adapterTmp = nullptr;
	IDXGIAdapter* adapterVender[Num_GPUVender] = { nullptr };	//各ベンダーのアダプター。
	IDXGIAdapter* adapterMaxVideoMemory = nullptr;				//最大ビデオメモリのアダプタ。
	IDXGIAdapter* useAdapter = nullptr;							//最終的に使用するアダプタ。
	SIZE_T videoMemorySize = 0;
	for (int i = 0; dxgiFactory->EnumAdapters(i, &adapterTmp) != DXGI_ERROR_NOT_FOUND; i++) {
		DXGI_ADAPTER_DESC desc;
		adapterTmp->GetDesc(&desc);
		
		if (desc.DedicatedVideoMemory > videoMemorySize) {
			//こちらのビデオメモリの方が多いので、こちらを使う。
			adapterMaxVideoMemory = adapterTmp;
			videoMemorySize = desc.DedicatedVideoMemory;
		}
		if (wcsstr(desc.Description, L"NVIDIA") != nullptr) {
			//NVIDIA製
			adapterVender[GPU_VenderNvidia] = adapterTmp;
		}
		else if (wcsstr(desc.Description, L"AMD") != nullptr) {
			//AMD製
			adapterVender[GPU_VenderAMD] = adapterTmp;
		}
		else if (wcsstr(desc.Description, L"Intel") != nullptr) {
			//Intel製
			adapterVender[GPU_VenderIntel] = adapterTmp;
		}
	}
	//使用するアダプターを決める。
	if (adapterVender[GPU_VenderNvidia] != nullptr) {
		//NVIDIA製が最優先
		useAdapter = adapterVender[GPU_VenderNvidia];
	}
	else if (adapterVender[GPU_VenderAMD] != nullptr) {
		//次はAMDが優先。
		useAdapter = adapterVender[GPU_VenderAMD];
	}
	else{
		//NVIDIAとAMDのGPUがなければビデオメモリが一番多いやつを使う。
		useAdapter = adapterMaxVideoMemory;
	}
	for (auto fuatureLevel : fuatureLevel) {

		auto hr = D3D12CreateDevice(
			useAdapter,
			fuatureLevel,
			IID_PPV_ARGS(&m_d3dDevice)
		);
		if (SUCCEEDED(hr)) {
			//D3Dデバイスの作成に成功した。
			break;
		}
	}
	return m_d3dDevice != nullptr;
}
bool GraphicsEngine::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	auto hr = m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(hr)) {
		//コマンドキューの作成に失敗した。
		return false;
	}
	return true;
}
bool GraphicsEngine::CreateSwapChain(
	HWND hwnd,
	UINT frameBufferWidth,
	UINT frameBufferHeight,
	IDXGIFactory4* dxgiFactory
)
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FRAME_BUFFER_COUNT;
	swapChainDesc.Width = frameBufferWidth;
	swapChainDesc.Height = frameBufferHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	IDXGISwapChain1* swapChain;
	dxgiFactory->CreateSwapChainForHwnd(
		m_commandQueue,
		hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	);
	//IDXGISwapChain3のインターフェースを取得。
	swapChain->QueryInterface(IID_PPV_ARGS(&m_swapChain));
	swapChain->Release();
	//IDXGISwapChain3のインターフェースを取得。
	m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	return true;
}
bool GraphicsEngine::CreateDescriptorHeapForFrameBuffer()
{
	//RTV用のディスクリプタヒープを作成する。
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FRAME_BUFFER_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	auto hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap));
	if (FAILED(hr)) {
		//RTV用のディスクリプタヒープの作成に失敗した。
		return false;
	}
	//ディスクリプタのサイズを取得。
	m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//DSV用のディスクリプタヒープを作成する。
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsvHeap));
	if (FAILED(hr)) {
		//DSV用のディスクリプタヒープの作成に失敗した。
		return false;
	}
	//ディスクリプタのサイズを取得。
	m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	return true;
}
bool GraphicsEngine::CreateRTVForFameBuffer()
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

	//フロントバッファをバックバッファ用のRTVを作成。
	for (UINT n = 0; n < FRAME_BUFFER_COUNT; n++) {
		m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
		m_d3dDevice->CreateRenderTargetView(
			m_renderTargets[n], nullptr, rtvHandle
		);
		rtvHandle.ptr += m_rtvDescriptorSize;
	}
	return true;
}
bool GraphicsEngine::CreateDSVForFrameBuffer( UINT frameBufferWidth, UINT frameBufferHeight )
{
	D3D12_CLEAR_VALUE dsvClearValue;
	dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	dsvClearValue.DepthStencil.Depth = 1.0f;
	dsvClearValue.DepthStencil.Stencil = 0;

	CD3DX12_RESOURCE_DESC desc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		frameBufferWidth,
		frameBufferHeight,
		1,
		1,
		DXGI_FORMAT_D32_FLOAT,
		1,
		0,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

	auto hr = m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&dsvClearValue,
		IID_PPV_ARGS(&m_depthStencilBuffer)
	);
	if (FAILED(hr)) {
		//深度ステンシルバッファの作成に失敗。
		return false;
	}
	//ディスクリプタを作成
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();

	m_d3dDevice->CreateDepthStencilView(
		m_depthStencilBuffer, nullptr, dsvHandle
	);

	return true;
}
bool GraphicsEngine::CreateCommandList()
{
	//コマンドリストの作成。
	m_d3dDevice->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList)
	);
	if (!m_commandList) {
		return false;
	}
	//コマンドリストは開かれている状態で作成されるので、いったん閉じる。
	m_commandList->Close();

	return true;
}
bool GraphicsEngine::CreateSynchronizationWithGPUObject()
{
	m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (!m_fence) {
		//フェンスの作成に失敗した。
		return false;
	}
	m_fenceValue = 1;
	//同期を行うときのイベントハンドラを作成する。
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr) {
		return false;
	}
	return true;
}
void GraphicsEngine::BeginRender(bool IsClear)
{
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	//カメラを更新する。
	m_camera2D.Update();
	m_camera3D.Update();

	//コマンドアロケータををリセット。
	m_commandAllocator->Reset();
	//レンダリングコンテキストもリセット。
	m_renderContext.Reset(m_commandAllocator, m_pipelineState);
	//ビューポートを設定。
	//ビューポートを設定。
	m_renderContext.SetViewport(m_viewport);
	//シザリング矩形を設定。
	m_renderContext.SetScissorRect(m_scissorRect);

	m_currentFrameBufferRTVHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	m_currentFrameBufferRTVHandle.ptr += m_frameIndex * m_rtvDescriptorSize;
	//深度ステンシルバッファのディスクリプタヒープの開始アドレスを取得。
	m_currentFrameBufferDSVHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//バックバッファがレンダリングターゲットとして設定可能になるまで待つ。
	m_renderContext.WaitUntilToPossibleSetRenderTarget(m_renderTargets[m_frameIndex]);

	//レンダリングターゲットを設定。
	if (IsClear) {
		m_renderContext.SetRenderTarget(m_currentFrameBufferRTVHandle, m_currentFrameBufferDSVHandle);
		m_renderContext.ClearRenderTargetView(m_currentFrameBufferRTVHandle, CLEARCOLOR);
		m_renderContext.ClearDepthStencilView(m_currentFrameBufferDSVHandle, 1.0f);
	}
}

void GraphicsEngine::ChangeRenderTargetToFrameBuffer(RenderContext& rc)
{
	rc.SetRenderTarget(m_currentFrameBufferRTVHandle, m_defferd.GetDSV());
	rc.SetViewport(m_viewport);
}
void GraphicsEngine::EndRender(bool ChangeTarget)
{
	// レンダリングターゲットへの描き込み完了待ち
	m_renderContext.WaitUntilFinishDrawingToRenderTarget(m_renderTargets[m_frameIndex]);

	//GPUにメモリを送信。GPUが使用を完了した場合API側でリサイクルされる。
	//イメージ的にはガベージコレクションで大丈夫なはず。
	m_directXTKGfxMemroy->Commit(m_commandQueue);

	//レンダリングコンテキストを閉じる。
	m_renderContext.Close();

	//コマンドを実行。
	ID3D12CommandList* ppCommandLists[] = { m_commandList };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
#ifdef SAMPE_16_04
	// Present the frame.
	m_swapChain->Present(0, 0);
#else
	// Present the frame.
	if (ChangeTarget) {
		//present(垂直同期を何回まつか、)
		m_swapChain->Present(1, 0);
	}

#endif
	//描画完了待ち。
	WaitDraw();
}