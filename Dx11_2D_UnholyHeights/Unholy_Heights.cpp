#include "stdafx.h"
#include "MainGame.h"

//접두어 _g ,_m
// n int,short
// l long
// d double
// w word
// b bool byte
// c const
// p 포인터
// lp 롱 포인터
// sz,str 문자열
// hwnd 윈도우 핸들
// h 리소스 핸들


// 전역 변수:
//실행되고 있는 프로그램의 메모리 시작주소를 가지고 있다.
//프로그램당 하나의 인스턴스를 가지고 관리함
//윈도우창을 관리하는 핸들
//윈도우창 여러개 사용 하는 경우 여러개의 핸들이 필요함
HINSTANCE	g_hInst;                                // 현재 인스턴스입니다.]
HWND		g_hwnd;

IDXGISwapChain*				SwapChain;
//하나 이상의 구현 표면 출력에 보내기 전에 렌더링 된 데이터를 저장한다.
//https://docs.microsoft.com/ko-kr/windows/win32/api/dxgi/nn-dxgi-idxgiswapchain

ID3D11Device*				Device;
//장치 인터페이스는 가상 어댑터를 나타냅니다. 자원을 작성하는 데 사용됩니다.
//기능 지원 점검과 자원 할당
//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nn-d3d11-id3d11device
ID3D11DeviceContext*		DeviceContext;
//인터페이스 명령어를 생성하는 렌더링 장치 컨텍스트를 나타낸다.
//렌더대상 설정,자원을 그래픽 파이프라인에 묶고 gpu가 수행할 렌더링 명령들을 지시
//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nn-d3d11-id3d11devicecontext



ID3D11RenderTargetView*		RTV;
D3DXVECTOR2		g_ptMouse = { 0.0f , 0.0f };
D3DXVECTOR2		g_ptCam = { 0.0f , 0.0f };

MainGame*	Mg;

vector<D3DXVECTOR2> upFloor;
vector<D3DXVECTOR2> downFloor;
KingRoom* kingDoor;
int playerGold;
short wheel = 0;
D3DXCOLOR backColor;
float gameTime;		//게임 내 자체 시간

//LPSTER -> char*
//LPCSTER -> const char*
//LPWSTR -> wchar*
//LPCWSTR -> const wchar*
//LPTSTR
//LPCTSTR



// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
//윈도우 프로시져 : 메세지를 운영체제에 전달 강제로 운영체제가 호출해준다
//hwnd :어느 윈도우에서 발생한 메세지인지 구분
//message : 메세지 구분 번호
//wparam : 마우스 버튼의 상태 키보드 조합키의 상태를 전달한다.
//lparam : 마우스 클릭 좌표 전달
//callball 어떤 이벤트가 발생했을때 나에 알려달라고 하는것
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void SetWindowSize(int x, int y, int width, int height);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR    lpCmdLine,
                      int       nCmdShow)
{


	WNDCLASS	wc;	//window infomation struct
	ZeroMemory(&wc, sizeof(WNDCLASS));

	// 참조하지 않은 인자에 대해 경고를 표시하지 않는다.
	UNREFERENCED_PARAMETER(lpCmdLine);

	//비트연산자   111
	//윈도우 클래스의 스타일을 지정
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//메세지를 처리하는 함수를 지정(프로시져)
	wc.lpfnWndProc = static_cast<WNDPROC>(WndProc);
	//윈도우 클래스 에서 사용하고자 하는 여분의 메모리 양을 바이트 단위로 지정
	wc.cbClsExtra = 0;
	//cbClsExtra 유사하되 개별 윈도우에서 사용하고자 하는 여분의 메모리양을 지정
	wc.cbWndExtra = 0;
	//윈도우 클래스를 등록한 응용 프로그램의 인스턴스 핸들
	wc.hInstance = hInstance;
	//클래스 커서를 지정
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//타이틀바의 좌상단에 보여줄 아이콘 윈도우가 최소화 되었을때 보여줄 아이콘을 지정
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//윈도우 작업영역에 칠할 배경 브러시
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
	//등록하고자 하는 윈도우의 이름
	wc.lpszClassName = WINNAME;
	//클래스로부터 만들어진 윈도우가 사용할 메뉴 지장
	wc.lpszMenuName = nullptr;

	//운영체제의 레지스터에 이 프로그램을 등록한다.
	if (RegisterClass(&wc) == false)
	{
		return FALSE;
	}

	//핸들 인스턴스 저장
	g_hInst = hInstance;
	g_hwnd = CreateWindow(
		WINNAME,				//윈도우 클래스의 이름
		WINNAME,				//윈도우 타이틀바 이름
		WS_OVERLAPPEDWINDOW,	//윈도우 스타일
		WINSTARTX,					    //창의 시작 x좌표
		WINSTARTY,						//창의 시작 y좌표
		WINSIZEX,					//창의 가로크기
		WINSIZEY,					//창의 세로크기
		nullptr,				//부모 윈도우
		nullptr,				//메뉴 핸들
		hInstance,				//인스턴스 지정
		nullptr
	);

	if (!g_hwnd)
	{
		return FALSE;
	}

	//Create Device and DeviceContext, SwapChain
	{
		DXGI_MODE_DESC bufferDesc;
		//desc 나오면 구조체로 보면된다. 디스플레이 모드를 설명합니다.
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

		bufferDesc.Width = WINSIZEX;// 원하는 back buffer 너비
		bufferDesc.Height = WINSIZEY;// 원하는 back buffer 높이
		bufferDesc.RefreshRate.Numerator = FIXFRAME; //새로 고침 빈도를 헤르츠 단위로 설명 하는 DXGI_RATIONAL 구조 분자
		bufferDesc.RefreshRate.Denominator = 1; // 분모
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// back buffer 픽셀 형식
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;// 디스플레이 스캔라인 모드
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//디스플레이 비례모드


		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

		desc.BufferDesc = bufferDesc;// 생성하고자 하는 back buffer의 속성들을 서술하는 구조체
		desc.SampleDesc.Count = 1;// Multisampling을 위해 추출할 표본 개수와 품질 수준을 서술하는 구조체
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;// 버퍼의 용도를 서술하는 구조체
		desc.BufferCount = 1; // Swap chain에서 사용할 back buffer의 개수.(이중버퍼링 : 1개, 삼중버퍼링 : 2개)
		desc.OutputWindow = g_hwnd;// 렌더링 결과를 표시할 윈도우 창의 핸들
		desc.Windowed = TRUE;// 창 모드를 원하면 true, 전체화면 모드를 원하면 false
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;// 추가적인 플래그

		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[7] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};



		HRESULT hr = D3D11CreateDeviceAndSwapChain
		(
			NULL,//표시할 '디스플레이 디바이스(비디오 카드)'의 'IDXGIAdapter 인터페이스'를 설정합니다.
			//이것은 DXGI의 인터페이스입니다. NULL을 지정하면 최초에 발견한 디바이스를 사용합니다. 
			//비디오카드를 지정할 필요가 있을때를 제외하고는 기본적으로 NULL로 설정하면 됩니다
			D3D_DRIVER_TYPE_HARDWARE,//드라이버 타입 D3D_DRIVER_TYPE_S0FTWARK,D3D_DRIVER_TYPE_REFERENCE 등이 있음
			NULL, //소프트웨어 구동기 지정 하드웨어타입으로 지정했을경우 NULL
			creationFlags, //추가적인 장치 생성 플래그들을 지정한다NULL 은 없음 D3D11_CREATE_DEVICE_DEBUG 등이 있음
			featureLevels, //기능수준 레벨 정하는 매개변수,NULL 최고레벨  D3D_FEATURE_LEVEL_11_0 등이 사용됨
			ARRAYSIZE(featureLevels), //기능수준 레벨의 D3D_FEATURE_LEVEL 원소 개수이다.널 값을 지정했다면 이 매개변수는 0으로 지정하면 된다
			D3D11_SDK_VERSION, //항상 D3D11_SDK_VERSI0N을 지정한다
			&desc,//스왑체인의 설정값들을 저장한 구조체의 포인터를 넘겨준다
			&SwapChain, //생성된 스왑체인 인터페이스의 포인터를 담을 변수
			&Device,	//생성된 디바이스 인터페이스의 포인터를 담을 변수를 설정합니다.
			NULL, //생성에 성공한 경우에는 pFeatureLevels에서 지정했던 배열의 처음값을 돌려줍니다.실패한 경우에는 0이 반환됩니다.
			&DeviceContext	//생성된 디바이스 컨텍스트 인터페이스의 포인터를 담을 변수를 설정합니다.
		);
		assert(SUCCEEDED(hr) && "디바이스스왑체인 생성 불가");
	}

	//Create BackBuffer
	{
		HRESULT hr;

		ID3D11Texture2D* BackBuffer;
		hr = SwapChain->GetBuffer(0, //접근할 버퍼의 번호(인덱스)
			__uuidof(ID3D11Texture2D), //백버퍼를 받을 인터페이스의 타입
			(void**)&BackBuffer);//반환된 인터페이스를 받을 변수의 포인터
		assert(SUCCEEDED(hr));

		hr = Device->CreateRenderTargetView(BackBuffer,//렌더타겟 뷰에서 엑세스하는 리소스
			NULL, //렌더타겟 뷰를 정의하는 'D3D11_RENDER_TARGET_VIEW_DESC'구조체의 포인터.
			//NULL을 넘겨주면, 리소스가 만들어졌을때의 포맷을 그대로 사용하며, 모든 리소스의 밉맵 레벨 0에 액세스하는 뷰를 생성한다.
			&RTV);//ID3D11RenderTargetView 인터페이스를 받는 변수의 포인터. NULL을 넘겨주게 되면, 다른 인자들의 유효성을 체크할 수 있다.

		assert(SUCCEEDED(hr));
		//렌더타겟 뷰가 생성된 이후에는 직접 백버퍼의 포인터에 접근하지 않고, 이 렌더타겟 뷰를 사용합니다. 
		//그렇기 때문에 렌더타겟 뷰의 인터페이스를 얻고 난 이후에는 백버퍼의 포인터를 해제합니다.
		SAFE_RELEASE(BackBuffer);
		


		//출력병합에 해당 렌더타겟뷰 수만큼 지정
		DeviceContext->OMSetRenderTargets(1,//렌더타겟이 몇개인지
			&RTV, //렌더할 대상배열들의 포인터
			NULL);//스텐실 깊이 지정 NULL이면 깊이값 꺼두기
	}

	

	//Create Viewport
	{
		D3D11_VIEWPORT viewport = { 0 };

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = WINSIZEX;
		viewport.Height = WINSIZEY;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		DeviceContext->RSSetViewports(1,//바인딩 할 뷰포트 수입니다.
			&viewport);//장치에 바인딩 할 D3D11_VIEWPORT 구조 의 배열입니다
	}


	//Disable DepthStencil
	{
		//해당 뎁스스텐실 기능관련부분은 꺼두는 옵션으로 설정
		D3D11_DEPTH_STENCIL_DESC desc = { 0 };
		desc.DepthEnable = false;
		desc.StencilEnable = false;

		ID3D11DepthStencilState* depthStencilState;
		Device->CreateDepthStencilState(&desc, &depthStencilState);

		DeviceContext->OMSetDepthStencilState(depthStencilState, 0xFF);
		SAFE_RELEASE(depthStencilState); 
	}
	//레스터라이저 옵션
	{
		ID3D11RasterizerState* rasterizerState;
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_WIREFRAME은 테두리만나옴
		desc.CullMode = D3D11_CULL_NONE; //컬링할때 앞면,뒷면 추려내기 할건지
		desc.DepthBias = 0;
		desc.FrontCounterClockwise = false; //cw ccw 값 정하기
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = false; //깊이클리핑 끄기
		desc.ScissorEnable = false; //시저테스트 하지 않음
		desc.MultisampleEnable = false; //멀티 샘플링 하지않음
		desc.AntialiasedLineEnable = false; //라인안티앨리어싱 없음
		Device->CreateRasterizerState(&desc, &rasterizerState);
		DeviceContext->RSSetState(rasterizerState);
		SAFE_RELEASE(rasterizerState);
	}
	//샘플러 관련 옵션
	{
		D3D11_SAMPLER_DESC samplerDesc;
		//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = -FLT_MAX;
		samplerDesc.MaxLOD = FLT_MAX;

		ID3D11SamplerState* myLinearWrapSampler;
		HRESULT hr = Device->CreateSamplerState(&samplerDesc, &myLinearWrapSampler);
		assert(SUCCEEDED(hr));
		DeviceContext->PSSetSamplers(0, 1, &myLinearWrapSampler);
		SAFE_RELEASE(myLinearWrapSampler);
	}

	//화면 작업 사이즈 영역 계산
	SetWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);


	Mg = new MainGame;
	//메인게임 클래스 초기화 실패했다면 바로 종료
	if (FAILED(Mg->init()))
	{
		return 0;
	}


	
	//생성한 프로그램을 디스플레이의 최상위로 올린다.
	ShowWindow(g_hwnd, nCmdShow);
	
	//메인 윈도우 갱신
	UpdateWindow(g_hwnd);

    MSG msg;
	ZeroMemory(&msg, sizeof(MSG));


	//TranslateMessage : 키보드 입력 메세지를 처리
	//입력키가 문자인지 확인 후 대문자,소문자,한글,영문 등등
	//WM_CHAR 메세지를 발생시킨다.

	//DispatchMessage : 윈도우 프로시져에 전달된 메세지를 실제 윈도우에 전달
	
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else
		{
			TIMEMANAGER->SetFrame(FIXFRAME);
			Mg->update();
			Mg->render();
		}
	}

	Mg->release();
	SAFE_DELETE(Mg);

	SAFE_RELEASE(SwapChain);
	SAFE_RELEASE(Device);
	SAFE_RELEASE(DeviceContext);
	SAFE_RELEASE(RTV);

	DestroyWindow(g_hwnd);
	UnregisterClass(WINNAME, hInstance);

    return static_cast<int> (msg.wParam);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//std::vector
	if (ImGui::WndProc((UINT*)hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_GETMINMAXINFO:
	{
		//윈도우 크기 고정
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = WINSIZEX + 17;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = WINSIZEY + 40;
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = WINSIZEX + 17;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = WINSIZEY + 40;
	}
	break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = static_cast<float>(LOWORD(lParam));
		g_ptMouse.y = static_cast<float>(WINSIZEY) - static_cast<float>(HIWORD(lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_MOUSEWHEEL:
		wheel = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		break;
	}

	return (DefWindowProc(hWnd, message, wParam, lParam));
}

void SetWindowSize(int x, int y, int width, int height)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;
	//실제 윈도우 크기 조정
	//AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU, false);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	//위 RECT 정보로 윈도우 사이즈 셋팅
	SetWindowPos(g_hwnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top),
		SWP_NOZORDER | SWP_NOMOVE);

}

