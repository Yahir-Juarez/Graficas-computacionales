// D3DWnd.cpp : Define el punto de entrada de la aplicación.
//

#include "ImageProcess.h"
#include "framework.h"
#include "D3DWnd.h"

#include "CGrhaphicsManager.h"

#define MAX_LOADSTRING 100

struct MODEL_VERTEX
{
    float x;
    float y;
    float z;
    float u;
    float v;
};

// Variables globales:
HWND g_hWnd;
HINSTANCE hInst;                                // instancia actual


Wstring szTitle;                  // Texto de la barra de título
Wstring szWindowClass;            // nombre de clase de la ventana principal

CGraphicsManager* g_GraphicsMan;

SPtr <VertexShader> g_pVertexShader;
SPtr <PixelShader> g_pPixelShader;

SPtr <IndexBuffer> g_pIndexBuffer;

SPtr<InputLayout> g_pInputLayout;
SPtr <VertexBuffer> g_pVertexBuffer;
SPtr<MODEL_VERTEX> g_pModelVertex;

SPtr<Texture2D> g_pTexture2D;
SPtr<SamplerState> g_pSampleState;
bool g_isRunning = true;
// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
BOOL                InitGraphicsAssets();
void                Render();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Colocar código aquí.

    szTitle.resize(MAX_LOADSTRING);
    szWindowClass.resize(MAX_LOADSTRING);

    // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, &szTitle[0], MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_D3DWND, &szWindowClass[0], MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realizar la inicialización de la aplicación:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D3DWND));

    //Init Graphics Managrer
    g_GraphicsMan = new CGraphicsManager(reinterpret_cast<void*>(g_hWnd), false, false, 1, 1);
    InitGraphicsAssets();

    MSG msg;
    memset(&msg, 0, sizeof(msg));

    //ImageProcess mjmnj;
    //mjmnj.createFile("Yoda.bmp");

    // Bucle principal de mensajes:


    while (g_isRunning)
    {
        while (PeekMessage(&msg, g_hWnd, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
         /*   if (msg.message == WM_QUIT)
            {
                isRunning = false;
            }*/
            /*graph.clearRenderTarget(graph.getRenderTargetView(), LinearColor(1, 0, 0, 1));
            graph.present();*/
        }
        Render();
    }


    return (int) msg.wParam;
}



//
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: Registra la clase de ventana.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    memset(&wcex, 0, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3DWND));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_D3DWND);
    wcex.lpszClassName  = &szWindowClass[0];
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: Guarda el identificador de instancia y crea la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Almacenar identificador de instancia en una variable global

   g_hWnd = CreateWindowExW(0,
                             &szWindowClass[0],
                             &szTitle[0],
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             0, 
                             CW_USEDEFAULT,
                             0,
                             nullptr,
                             nullptr,
                             hInstance,
                             nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: Procesa mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizar las selecciones de menú:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Agregar cualquier código de dibujo que use hDC aquí...

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        g_isRunning = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//BOOL InitGraphicsAssets()
//{
//    /*
//    bloque de creacion.
//    */
//    g_pVertexShader = g_GraphicsMan->createVertexShader("VertexShader.hlsl", "main", "vs_5_0");
//    g_pPixelShader = g_GraphicsMan->createPixelShader("VertexShader.hlsl", "mainPS", "ps_5_0");
//
//    Vector<D3D11_INPUT_ELEMENT_DESC> vInputElements;
//
//    vInputElements.resize(2);
//    
//        auto& posElements = vInputElements[0];
//        memset(&posElements, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));
//    
//        posElements.SemanticName = "POSITION";
//        posElements.SemanticIndex = 0;
//        posElements.Format = DXGI_FORMAT_R32G32B32_FLOAT;
//        posElements.InputSlot = 0;
//        posElements.AlignedByteOffset = 0;
//        posElements.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//        posElements.InstanceDataStepRate = 0;
//    
//        auto& posElements1 = vInputElements[1];
//        memset(&posElements1, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));
//    
//        posElements1.SemanticName = "TEXCOORD";
//        posElements1.SemanticIndex = 0;
//        posElements1.Format = DXGI_FORMAT_R32G32B32_FLOAT;
//        posElements1.InputSlot = 0;
//        posElements1.AlignedByteOffset = 12;
//        posElements1.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//        posElements1.InstanceDataStepRate = 0;
//    
//        g_pInputLayout = g_GraphicsMan->createInputLayout(vInputElements, g_pVertexShader);
//
//    Vector<MODEL_VERTEX> mesh;
//    mesh.push_back({ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f });
//    mesh.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f });
//    mesh.push_back({ 0.5f, 0.5f, 0.8f, 1.0f, 1.0f });
//    mesh.push_back({ -0.5f, 0.5f, 0.8f, 0.0f, 1.0f });
//
//    mesh.push_back({ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f });
//    mesh.push_back({ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f });
//    mesh.push_back({ 0.5f, -0.5f, 0.8f, 1.0f, 1.0f });
//    mesh.push_back({ -0.5f, -0.5f, 0.8f, 0.0f, 1.0f });
//
//    mesh.push_back({ -0.5f, -0.5f, 0.8f, 0.0f, 0.0f });
//    mesh.push_back({ -0.5f, -0.5f, 0.5f, 1.0f, 0.0f });
//    mesh.push_back({ -0.5f, 0.5f, 0.5f, 1.0f, 1.0f });
//    mesh.push_back({ -0.5f, 0.5f, 0.8f, 0.0f, 1.0f });
//
//    mesh.push_back({ 0.5f, -0.5f, 0.8f, 0.0f, 0.0f });
//    mesh.push_back({ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f });
//    mesh.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f });
//    mesh.push_back({ 0.5f, 0.5f, 0.8f, 0.0f, 1.0f });
//
//    mesh.push_back({ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f });
//    mesh.push_back({ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f });
//    mesh.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f });
//    mesh.push_back({ -0.5f, 0.5f, 0.5f, 0.0f, 1.0f });
//
//    mesh.push_back({ -0.5f, -0.5f, 0.8f, 0.0f, 0.0f });
//    mesh.push_back({ 0.5f, -0.5f, 0.8f, 1.0f, 0.0f });
//    mesh.push_back({ 0.5f, 0.5f, 0.8f, 1.0f, 1.0f });
//    mesh.push_back({ -0.5f, 0.5f, 0.8f, 0.0f, 1.0f });
//
//    g_pVertexBuffer = g_GraphicsMan->createVertexBuffer<MODEL_VERTEX>(mesh);
//
//    Vector<uint32> indices
//    {
//        3, 1, 0,
//        2, 1, 3,
//
//        6, 4, 5,
//        7, 4, 6,
//
//        11, 9, 8,
//        10, 9, 11,
//
//        14, 12, 13,
//        15, 12, 14,
//
//        19, 17, 16,
//        18, 17, 19,
//
//        22, 20, 21,
//        23, 20, 22
//    };
//
//    g_pIndexBuffer = g_GraphicsMan->createIndexBuffer(indices);
//
//    g_pTexture2D = g_GraphicsMan->createTexture2DFromFile("Yoda.bmp");
//    g_pSampleState = g_GraphicsMan->CreateSampleState();
//    return TRUE;
//
//}


BOOL InitGraphicsAssets()
{
    /*
    Bloque de creacion.
    */
    g_pVertexShader = g_GraphicsMan->createVertexShader("VertexShader.hlsl", "main", "vs_5_0");
    g_pPixelShader = g_GraphicsMan->createPixelShader("VertexShader.hlsl", "mainPS", "ps_5_0");

    Vector<D3D11_INPUT_ELEMENT_DESC> vInputElements;

    vInputElements.resize(2);

    auto& posElements = vInputElements[0];
    memset(&posElements, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));

    posElements.SemanticName = "POSITION";
    posElements.SemanticIndex = 0;
    posElements.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    posElements.InputSlot = 0;
    posElements.AlignedByteOffset = 0;
    posElements.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    posElements.InstanceDataStepRate = 0;

    auto& posElements1 = vInputElements[1];
    memset(&posElements1, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));

    posElements1.SemanticName = "TEXCOORD";
    posElements1.SemanticIndex = 0;
    posElements1.Format = DXGI_FORMAT_R32G32_FLOAT;
    posElements1.InputSlot = 0;
    posElements1.AlignedByteOffset = 12;
    posElements1.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    posElements1.InstanceDataStepRate = 0;

    g_pInputLayout = g_GraphicsMan->createInputLayout(vInputElements, g_pVertexShader);

    Vector<MODEL_VERTEX> mesh;
    mesh.push_back({ 0.0f, 0.5f, 0.5f, 0.5f, 0.0f });
    mesh.push_back({ 0.5f, -0.5f, 0.5f, 1.0f, 1.0f });
    mesh.push_back({ -0.5f, -0.5f, 0.5f,  0.0f, 1.0f });

    g_pVertexBuffer = g_GraphicsMan->createVertexBuffer<MODEL_VERTEX>(mesh);


    g_pTexture2D = g_GraphicsMan->createTexture2DFromFile("Yoda.bmp");
    g_pSampleState = g_GraphicsMan->CreateSampleState();
    return TRUE;

}

void Render()
{
    /*
   * Bloque de render
   */
    g_GraphicsMan->clearRenderTarget(g_GraphicsMan->getRenderTargetView(), LinearColor(0, 0, 1, 1));

    UINT offset = 0;
    
    g_GraphicsMan->setRenderTargets();
    g_GraphicsMan->setInputLayout(g_pInputLayout);
    g_GraphicsMan->setVertexBuffers(g_pVertexBuffer, offset);
    //g_GraphicsMan->setIndexBuffers(g_pIndexBuffer);
    g_GraphicsMan->setPrimitiveTopology();
    g_GraphicsMan->setVertexShader(g_pVertexShader);
    g_GraphicsMan->setPixelShader(g_pPixelShader);
    g_GraphicsMan->setShaderResources(g_pTexture2D);
    g_GraphicsMan->setSamplers(g_pSampleState);

    g_GraphicsMan->Draw(3, 0);

    g_GraphicsMan->present();
}