// D3DWnd.cpp : Define el punto de entrada de la aplicación.
//

#include "ImageProcess.h"
#include "framework.h"
#include "D3DWnd.h"
#include "myMatrix.h"
#include "CGrhaphicsManager.h"
#include "Camera.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define MAX_LOADSTRING 100



// Variables globales:
HWND g_hWnd;
HINSTANCE hInst;                                // instancia actual


Wstring szTitle;                  // Texto de la barra de título
Wstring szWindowClass;            // nombre de clase de la ventana principal

CGraphicsManager* g_GraphicsMan;

SPtr <VertexShader> g_pVertexShader;
SPtr <PixelShader> g_pPixelShader;

Vector<SPtr <IndexBuffer>> g_pIndexBuffer;

SPtr<InputLayout> g_pInputLayout;
Vector<SPtr<VertexBuffer>> g_pVertexBuffer;

SPtr<ConstantBuffer> g_pWVP;
Vector<SPtr<MODEL_VERTEX>> g_pModelVertex;

SPtr<Texture2D> g_pTexture2D;
SPtr<SamplerState> g_pSampleState;
Vector<MODEL_VERTEX> g_Mesh;

POINT g_MousePos;
POINT g_MousePosLast;

struct WVP
{
    MyMatrix4 view;
    MyMatrix4 proj;
};

WVP myWVP;

bool g_isRunning = true;
CameraFPS g_MainCamera;
// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
BOOL                InitGraphicsAssets();
void                Render();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void                moveCamera();
void                moveCameraRotate();
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


    float time = 0.0f;
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
        //time = gettick
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
    case WM_KEYDOWN:
        moveCamera();
        break;
    case WM_MOUSEMOVE:
    {
        g_MousePosLast = g_MousePos;

        g_MousePos.x = GET_X_LPARAM(lParam);
        g_MousePos.y = GET_Y_LPARAM(lParam);

        if (g_MousePosLast.x - g_MousePos.x != 0.0f || g_MousePosLast.y - g_MousePos.y != 0.0f)
        {
            g_MainCamera.rotate((g_MousePosLast.y - g_MousePos.y) * 0.005f, (g_MousePosLast.x - g_MousePos.x) * 0.005f);
        }

        /////////////////////Test rotacion///////////////////////////////
        //if (g_MousePosLast.x - g_MousePos.x != 0.0f || g_MousePosLast.y - g_MousePos.y != 0.0f)
        //{
        //    //g_MainCamera.rotate((g_MousePosLast.y - g_MousePos.y) * 0.000005f, (g_MousePosLast.x - g_MousePos.x) * 0.000005f);
        //    if (g_MousePosLast.y - g_MousePos.y > 0)
        //    {
        //        g_MainCamera.rotate(0.001, 0.0f);
        //    }
        //    else
        //    {
        //        g_MainCamera.rotate(-0.001, 0.0f);
        //    }
        //    if ((g_MousePosLast.x - g_MousePos.x) > 0)
        //    {
        //        g_MainCamera.rotate(0, 0.001f);
        //    }
        //    else
        //    {
        //        g_MainCamera.rotate(0, -0.001f);
        //    }
        //}
    ////////////////////////////////////////////////////////
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

    case WM_SIZE:
    {
        RECT rc;
        GetClientRect(g_hWnd, &rc);
        g_MainCamera.setProjData(45.0f, static_cast<float>(rc.right), static_cast<float>(rc.bottom), 0.1f, 100.0f);
    }
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
//Basic usage(see HDR discussion below for HDR usage) :
    //    int x,y,n;
    //    unsigned char *data = stbi_load(filename, &x, &y, &n, 4); 
BOOL InitGraphicsAssets()
{
    /*
    bloque de creacion.
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
    
    Vector<void*> vpInputLayout;
    for (int i = 0; i < vInputElements.size(); i++)
    {
        vpInputLayout.push_back(&vInputElements[i]);
    }
    
    g_pInputLayout = g_GraphicsMan->createInputLayout(vpInputLayout, g_pVertexShader);

    /////////////Carga modelo////////////

    g_GraphicsMan->loadModel("Walking.fbx", g_Mesh, g_pIndexBuffer, g_pVertexBuffer);
    //g_GraphicsMan->loadModel("Bistro_Research_Interior.fbx", g_Mesh, g_pIndexBuffer, g_pVertexBuffer);

    /////////////////////////////////////
    int sizeoM = sizeof(MyMatrix4);

    g_pWVP = g_GraphicsMan->createConsantBuffer(sizeoM * 2);

    g_pTexture2D = g_GraphicsMan->createTexture2DFromFile("Test2Texture.bmp");
    g_pSampleState = g_GraphicsMan->CreateSampleState();

    RECT rc;
    GetClientRect(g_hWnd, &rc);

    g_MainCamera.setViewData(myVector3(0.0f, 0.0f, 50.0f), myVector3(0.0f, 0.0f, 0.0f), myVector3(0.0f, 1.0f, 0.0f));
    g_MainCamera.setProjData((3.1416f/4.0f), static_cast<float>(rc.right), static_cast<float>(rc.bottom), 0.1f, 800.0f);


    

    //Utiliza direct3d matematica intrinsica, para open GL no se necesaita la transpuesta

    myWVP.view = g_MainCamera.m_viewMatrix.GetTransposed();
    myWVP.proj = g_MainCamera.m_projMatrix.GetTransposed();

    g_GraphicsMan->updateConstantBuffer(g_pWVP, &myWVP, sizeof(myWVP));

    return TRUE;

}

//y = mx + b

//BOOL InitGraphicsAssets()
//{
//    /*
//    Bloque de creacion.
//    */
//    g_pVertexShader = g_GraphicsMan->createVertexShader("VertexShader.hlsl", "main", "vs_5_0");
//    g_pPixelShader = g_GraphicsMan->createPixelShader("VertexShader.hlsl", "mainPS", "ps_5_0");
//
//    Vector<D3D11_INPUT_ELEMENT_DESC> vInputElements;
//
//    vInputElements.resize(2);
//
//    auto& posElements = vInputElements[0];
//    memset(&posElements, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));
//
//    posElements.SemanticName = "POSITION";
//    posElements.SemanticIndex = 0;
//    posElements.Format = DXGI_FORMAT_R32G32B32_FLOAT;
//    posElements.InputSlot = 0;
//    posElements.AlignedByteOffset = 0;
//    posElements.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//    posElements.InstanceDataStepRate = 0;
//
//    auto& posElements1 = vInputElements[1];
//    memset(&posElements1, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));
//
//    posElements1.SemanticName = "TEXCOORD";
//    posElements1.SemanticIndex = 0;
//    posElements1.Format = DXGI_FORMAT_R32G32_FLOAT;
//    posElements1.InputSlot = 0;
//    posElements1.AlignedByteOffset = 12;
//    posElements1.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//    posElements1.InstanceDataStepRate = 0;
//
//    g_pInputLayout = g_GraphicsMan->createInputLayout(vInputElements, g_pVertexShader);
//
//    Vector<MODEL_VERTEX> mesh;
//    mesh.push_back({ 0.0f, 0.5f, 0.5f, 0.5f, 0.0f });
//    mesh.push_back({ 0.5f, -0.5f, 0.5f, 1.0f, 1.0f });
//    mesh.push_back({ -0.5f, -0.5f, 0.5f,  0.0f, 1.0f });
//
//    g_pVertexBuffer = g_GraphicsMan->createVertexBuffer<MODEL_VERTEX>(mesh);
//
//
//    g_pTexture2D = g_GraphicsMan->createTexture2DFromFile("StarWars_Mando.bmp");
//    g_pSampleState = g_GraphicsMan->CreateSampleState();
//    return TRUE;
//
//}

//MyMatrix4 LookAt(const myVector3& CameraPos,
//    const myVector3& ObjectivePos,
//    const myVector3& UpVector)
//{
//    const myVector3 ZAxis = (ObjectivePos - CameraPos).getVectorNormalize();
//    const myVector3 XAxis = (UpVector ^ ZAxis).getVectorNormalize();
//    const myVector3 YAxis = ZAxis ^ XAxis;
//
//    MyMatrix4 result;
//
//    for (uint32 RowIndex = 0; RowIndex < 3; ++RowIndex)
//    {
//        result.m_matrix[RowIndex][0] = (&XAxis.x)[RowIndex];
//        result.m_matrix[RowIndex][1] = (&YAxis.x)[RowIndex];
//        result.m_matrix[RowIndex][2] = (&ZAxis.x)[RowIndex];
//        result.m_matrix[RowIndex][3] = 0.0f;
//    }
//
//    myVector3 negCameraPos = CameraPos;
//    negCameraPos.x = negCameraPos.x * -1;
//    negCameraPos.y = negCameraPos.y * -1;
//    negCameraPos.z = negCameraPos.z * -1;
//
//    result.m_matrix[3][0] = negCameraPos | XAxis;
//
//    return result;
//}

bool isKeyPressed(char key)
{
    return GetAsyncKeyState(key) & 0x8000;
}

void moveCamera()
{
    myVector3 camMove{0.0f, 0.0f, 0.0f};

    float speed = 1.0f;

    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
    {
        speed = 3.0f;
    }
    if (isKeyPressed('A')) 
    {
        camMove.x = speed;
    }
    else if (isKeyPressed('D'))
    {
        camMove.x = -speed;
    }
    if (isKeyPressed('W'))
    {
        camMove.z = speed;
    }
    else if (isKeyPressed('S'))
    {
        camMove.z = -speed;
    }
    if (isKeyPressed('Q'))
    {
        camMove.y = speed;
    }
    else if (isKeyPressed('E'))
    {
        camMove.y = -speed;
    }
    g_MainCamera.moveForward(camMove.z);
    g_MainCamera.moveRight(camMove.x);
    g_MainCamera.moveUp(camMove.y);
   /* if (isKeyPressed('X'))
    {
        g_MainCamera.rotate(0.1f, 0.0f);
    }
    if (isKeyPressed('Y'))
    {
        g_MainCamera.rotate(0.0f, -0.1f);
    }
    if (isKeyPressed('Z'))
    {
        g_MainCamera.rotate(-0.1f, 0.0f);
    }
    if (isKeyPressed('T'))
    {
        g_MainCamera.rotate(0.0f, 0.1f);
    }*/
}

void moveCameraRotate()
{
}


void Render()
{
    /*
   * Bloque de render
   */
    g_GraphicsMan->clearRenderTarget(g_GraphicsMan->getRenderTargetView(), LinearColor(0, 0, 1, 1));
    g_GraphicsMan->clearRenderTargetView(g_GraphicsMan->getMainDSV());

    UINT offset = 0;
    for (int i = 0; i < g_pIndexBuffer.size(); i++)
    {
        g_GraphicsMan->setRenderTargets(1, g_GraphicsMan->getMainDSV());
        g_GraphicsMan->setInputLayout(g_pInputLayout);
        g_GraphicsMan->setVertexBuffers(g_pVertexBuffer[i], offset);
        g_GraphicsMan->setIndexBuffers(g_pIndexBuffer[i]);
        g_GraphicsMan->setPrimitiveTopology();
        g_GraphicsMan->setVertexShader(g_pVertexShader);
        g_GraphicsMan->setPixelShader(g_pPixelShader);
        g_GraphicsMan->setShaderResources(g_pTexture2D);
        g_GraphicsMan->setSamplers(g_pSampleState);

        //g_pWVP->getBuffer();
        g_GraphicsMan->vsSetConstantBuffers(g_pWVP);
        g_GraphicsMan->DrawIndex(g_GraphicsMan->m_index[i], 0, 0);
    }
    myWVP.view = g_MainCamera.m_viewMatrix.GetTransposed();
    myWVP.proj = g_MainCamera.m_projMatrix.GetTransposed();

    g_GraphicsMan->updateConstantBuffer(g_pWVP, &myWVP, sizeof(myWVP));

    g_GraphicsMan->present();
}


void cargaModelo()
{
    //////////////////////////////////////////////////// CARGA MODELO ////////////////////////////////////////////

    /*g_Mesh.push_back({ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f });
    g_Mesh.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f });
    g_Mesh.push_back({ 0.5f, 0.5f, 0.8f, 1.0f, 1.0f });
    g_Mesh.push_back({ -0.5f, 0.5f, 0.8f, 0.0f, 1.0f });

    g_Mesh.push_back({ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f });
    g_Mesh.push_back({ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f });
    g_Mesh.push_back({ 0.5f, -0.5f, 0.8f, 1.0f, 1.0f });
    g_Mesh.push_back({ -0.5f, -0.5f, 0.8f, 0.0f, 1.0f });

    g_Mesh.push_back({ -0.5f, -0.5f, 0.8f, 0.0f, 0.0f });
    g_Mesh.push_back({ -0.5f, -0.5f, 0.5f, 1.0f, 0.0f });
    g_Mesh.push_back({ -0.5f, 0.5f, 0.5f, 1.0f, 1.0f });
    g_Mesh.push_back({ -0.5f, 0.5f, 0.8f, 0.0f, 1.0f });

    g_Mesh.push_back({ 0.5f, -0.5f, 0.8f, 0.0f, 0.0f });
    g_Mesh.push_back({ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f });
    g_Mesh.push_back({ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f });
    g_Mesh.push_back({ 0.5f, 0.5f, 0.8f, 0.0f, 1.0f });

    g_Mesh.push_back({ -0.5f, -0.5f, 0.5f, 1.0f, 1.0f });
    g_Mesh.push_back({ 0.5f, -0.5f, 0.5f, 0.0f, 1.0f });
    g_Mesh.push_back({ 0.5f, 0.5f, 0.5f, 0.0f, 0.0f });
    g_Mesh.push_back({ -0.5f, 0.5f, 0.5f, 1.0f, 0.0f });

    g_Mesh.push_back({ -0.5f, -0.5f, 0.8f, 0.0f, 0.0f });
    g_Mesh.push_back({ 0.5f, -0.5f, 0.8f, 1.0f, 0.0f });
    g_Mesh.push_back({ 0.5f, 0.5f, 0.8f, 1.0f, 1.0f });
    g_Mesh.push_back({ -0.5f, 0.5f, 0.8f, 0.0f, 1.0f });*/

    //Vector<uint32> indices;
    ///*
    //{
    //    3,1,0,
    //    2,1,3,

    //    6,4,5,
    //    7,4,6,

    //    11,9,8,
    //    10,9,11,

    //    14,12,13,
    //    15,12,14,

    //    19,17,16,
    //    18,17,19,

    //    22,20,21,
    //    23,20,22
    //};*/

    //Assimp::Importer aImporter;
    //const aiScene* pScene = aImporter.ReadFile("Test2.fbx", aiProcessPreset_TargetRealtime_MaxQuality);

    //for (int i = 0; i < pScene->mNumMeshes; ++i) {
    //    aiMesh* mesh = pScene->mMeshes[i];

    //    for (int j = 0; j < mesh->mNumVertices; ++j) {
    //        aiVector3D vertex = mesh->mVertices[j];
    //        float posX = vertex.x;
    //        float posY = vertex.y;
    //        float posZ = vertex.z;
    //        aiVector3D texCoord = mesh->mTextureCoords[0][j]; 
    //        float posU = texCoord.x;
    //        float posV = texCoord.y;
    //        g_Mesh.push_back({ posX, posY, posZ, posU, posV });
    //    }

    //    uint32 numVertex = 0; //codigo todo pitero alch <3 tqm yair :b
    //    for (int j = 0; j < mesh->mNumFaces; ++j) {
    //        aiFace face = mesh->mFaces[j];
    //        for (int k = 0; k < face.mNumIndices; ++k) {
    //            uint32 indexIter = face.mIndices[k];
    //            indices.push_back(indexIter);
    //            numVertex++;
    //        }
    //    }
    //    g_GraphicsMan->m_index.push_back(numVertex);
    //    g_pVertexBuffer.push_back(g_GraphicsMan->createVertexBuffer<MODEL_VERTEX>(g_Mesh));
    //    g_pIndexBuffer.push_back( g_GraphicsMan->createIndexBuffer(indices));
    //    g_Mesh.clear();
    //}

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    bool x;
}