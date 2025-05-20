#ifndef _dxrr
#define _dxrr
// Declaración de las coordenadas como variables globales
extern float CordX, CordY, CordZ;
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "TerrenoRR.h"
#include "Camara.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "ModeloRR.h"
#include "XACT3Util.h"

#include <d2d1.h>
#include <dwrite.h>
#include <algorithm>
#include <string>
#include <vector>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

// Incluir las bibliotecas necesarias
#include <windows.h>
#include <dxgi.h>
#include <iostream>
#include "Text.h"
#include "GUI.h"

class DXRR {

private:
	int ancho;
	int alto;
	bool dentroTorre = false;
	bool dentroCastillo = false;
public:
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState* alphaBlendState, * commonBlendState;

	// Declaraciones globales
	ID2D1Factory* pD2DFactory = nullptr;
	IDWriteFactory* pDWriteFactory = nullptr;
	ID2D1RenderTarget* pRenderTarget = nullptr;
	ID2D1SolidColorBrush* pBrush = nullptr;
	IDWriteTextFormat* pTextFormat = nullptr;

	ID3D11Texture2D* uiRenderTargetTexture;
	ID3D11RenderTargetView* uiRenderTargetView;

	int frameBillboard;

	TerrenoRR* terreno;
	SkyDome* skydome;
	BillboardRR* billboard;
	Camara* camara;
	// Modelos
	ModeloRR* arbolQuemado;
	ModeloRR* carreta;
	ModeloRR* casa1;
	ModeloRR* casa2;
	ModeloRR* castillo;
	ModeloRR* castilloInterior;
	ModeloRR* dragon;
	ModeloRR* mesa;
	ModeloRR* pozo;
	ModeloRR* torre;
	ModeloRR* vaca;

	float izqder;
	float arriaba;
	float vel;
	float strafe = 0;
	bool breakpoint;
	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];

	XACTINDEX cueIndex;
	CXACT3Util m_XACT3;

	float coordenadas[3] = { 0.0f, 0.0f, 0.0f };

	// Variables logica de colisiones
	float danio = 0;
	int vida = 4;
	WCHAR* rutaVida = L"";
	WCHAR* rutaVidaHud = L"";

	// Skybox
	float counterDiaNoche;
	float gradient;

	// GUI
	GUI* HUD1;
	GUI* HUD2;
	GUI* VIDA;

	Text* textPt;
	string Puntos = "0";
	Text* textAt;
	string Ataque = "0";
	Text* textEn;
	string Enemigos = "0";


	DXRR(HWND hWnd, int Ancho, int Alto)
	{
		// Inicializaciones varias
		breakpoint = false;
		frameBillboard = 0;
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		izqder = 0;
		arriaba = 0;

		// Variables para jugabilidad
		danio = 0;
		vida = 4;
		rutaVida = L"Assets/Billboards/vida4.jpg";
		rutaVidaHud = L"Assets/GUI/vida4.png";

		// Variables para skybox
		counterDiaNoche = 0;
		gradient = 0;

		// Iniciar escena
		IniciaD3D(hWnd);
		IniciaDirect2D(hWnd);

		// Camara
		camara = new Camara(D3DXVECTOR3(0, 80, 6), D3DXVECTOR3(0, 80, 0), D3DXVECTOR3(0, 1, 0), Ancho, Alto);

		// Terreno
		terreno = new TerrenoRR(300, 300, d3dDevice, d3dContext);

		// Skydome
		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"Assets/Skybox/dia.jpg", L"Assets/Skybox/noche.png");

		// Modelos
		// (device, context, obj, textura, specular, pos X, pos Y)
		arbolQuemado = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/arbolQuemado.obj", L"Assets/Modelos/arbolQuemado.png", L"Assets/Modelos/NoSpecular.png", -90.0f, 60.0f);
		carreta = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/carreta1.obj", L"Assets/Modelos/carreta.png", L"Assets/Modelos/NoSpecular.png", 60.0f, -90.0f);
		casa1 = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/casa1.obj", L"Assets/Modelos/casa1.png", L"Assets/Modelos/NoSpecular.png", 80.0f, -110.0f);
		casa2 = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/casa2.obj", L"Assets/Modelos/casa2.png", L"Assets/Modelos/NoSpecular.png", -115.0f, -115.0f);
		castillo = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Castillo.obj", L"Assets/Modelos/ladrilloCOMB.jpg", L"Assets/Modelos/NoSpecular.png", 90.0f, 110.0f);
		castilloInterior = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/CastilloInterior.obj", L"Assets/Modelos/ladrilloCOMB.jpg", L"Assets/Modelos/NoSpecular.png", 90.0f, 110.0f);
		dragon = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/dragon1.obj", L"Assets/Modelos/dragon1.png", L"Assets/Modelos/NoSpecular.png", 130.0f, -40.0f);
		mesa = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/mesa.obj", L"Assets/Modelos/mesa.png", L"Assets/Modelos/NoSpecular.png", -100.0f, 100.0f);
		pozo = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/pozo.obj", L"Assets/Modelos/pozo.png", L"Assets/Modelos/NoSpecular.png", -50, -125);
		torre = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/torre.obj", L"Assets/Modelos/torre.png", L"Assets/Modelos/NoSpecular.png", 120, -110);
		vaca = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/vaca.obj", L"Assets/Modelos/vaca.png", L"Assets/Modelos/NoSpecular.png", -70, -130);

		inicializarBillboards();
		inicializarGUI();
	}

	void inicializarBillboards()
	{
		billboard = new BillboardRR(rutaVida, L"Assets/Billboards/NoNormal.jpg", d3dDevice, d3dContext, 1);
	}

	void inicializarGUI()
	{
		HUD1 = new GUI(d3dDevice, d3dContext, 0.5f, 0.5f, L"Assets/GUI/hud_layout_1.png");
		HUD2 = new GUI(d3dDevice, d3dContext, 0.5f, 0.5f, L"Assets/GUI/hud_layout_2.png");
		VIDA = new GUI(d3dDevice, d3dContext, 0.4f, 0.4f, rutaVidaHud);

		textPt = new Text(d3dDevice, d3dContext, 3.6f, 1.2f, L"Assets/GUI/font_2.png", XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
		textAt = new Text(d3dDevice, d3dContext, 3.6f, 1.2f, L"Assets/GUI/font_2.png", XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
		textEn = new Text(d3dDevice, d3dContext, 3.6f, 1.2f, L"Assets/GUI/font_2.png", XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	}

	~DXRR()
	{
		LiberaD3D();
		m_XACT3.Terminate();
	}

	void IniciaDirect2D(HWND hWnd)
	{
		HRESULT hr;

		// Inicializar la fábrica de Direct2D
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
		if (FAILED(hr))
		{
			MessageBox(hWnd, L"Error al crear la fábrica de Direct2D", L"Error", MB_OK);
			return;
		}

		// Crear un Bitmap Render Target
		D2D1_SIZE_U size = D2D1::SizeU(ancho, alto);

		D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
		rtProps.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
		rtProps.pixelFormat = D2D1_PIXEL_FORMAT{ DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE };
		rtProps.dpiX = 96.0f;
		rtProps.dpiY = 96.0f;

		hr = pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, size),
			reinterpret_cast<ID2D1HwndRenderTarget**>(&pRenderTarget)
		);

		if (FAILED(hr))
		{
			MessageBox(hWnd, L"Error al crear el Hwnd Render Target", L"Error", MB_OK);
			return;
		}

		// Crear la fábrica de DirectWrite
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));
		if (FAILED(hr))
		{
			MessageBox(hWnd, L"Error al crear la fábrica de DirectWrite", L"Error", MB_OK);
			return;
		}

		// Crear formato de texto
		hr = pDWriteFactory->CreateTextFormat(
			L"Arial",            // Tipo de fuente
			nullptr,             // Fuente de recursos (null para usar la predeterminada)
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			12.0f,               // Tamaño de fuente
			L"es-ES",            // Localización
			&pTextFormat
		);

		if (FAILED(hr))
		{
			MessageBox(hWnd, L"Error al crear el formato de texto", L"Error", MB_OK);
			return;
		}

		// Crear un pincel para el texto
		hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBrush);
		if (FAILED(hr))
		{
			MessageBox(hWnd, L"Error al crear el pincel de texto", L"Error", MB_OK);
			return;
		}
	}

	bool IniciaD3D(HWND hWnd)
	{
		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for (driver = 0; driver < totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels,
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if (SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if (FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}

		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if (FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if (backBufferTexture)
			backBufferTexture->Release();

		if (FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}

		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = width;
		texDesc.Height = heigth;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		d3dDevice->CreateTexture2D(&texDesc, NULL, &uiRenderTargetTexture);
		d3dDevice->CreateRenderTargetView(uiRenderTargetTexture, NULL, &uiRenderTargetView);

		result = d3dDevice->CreateTexture2D(&texDesc, NULL, &uiRenderTargetTexture);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la textura para UI", MB_OK);
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(uiRenderTargetTexture, NULL, &uiRenderTargetView);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el render target view para UI", MB_OK);
			return false;
		}

		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;

		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);

		return true;

	}

	void LiberaD3D(void)
	{
		if (depthTexture)
			depthTexture->Release();
		if (depthStencilView)
			depthStencilView->Release();
		if (backBufferTarget)
			backBufferTarget->Release();
		if (swapChain)
			swapChain->Release();
		if (d3dContext)
			d3dContext->Release();
		if (d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;


	}

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	void Render()
	{
		if (d3dContext == 0) return;
		UpdateSceneVariables(); // Opcional
		ClearBuffers();         // Limpieza
		UpdateCamera();         // Movimiento y ajuste de altura
		RenderSkydome();        // Cielo sin profundidad 
		RenderScene();          // Escena 3D
		logicCollision();		// Implementa la logica de las colisiones
		PresentFrame();         // Mostrar frame
		/*RenderUI();*/
	}

	void UpdateSceneVariables()
	{
		static float angle = 0.0f;
		angle += 0.005f;
		if (angle >= 360.0f) angle = 0.0f;

		float sphere[3] = { 0, 0, 0 };
		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		bool collide = false;

	}


	void ClearBuffers()
	{
		// Limpiar el fondo
		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView(backBufferTarget, clearColor);
		d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void UpdateCamera()
	{
		// Actualizar posición temporal
		camara->UpdateCam(vel, arriaba, izqder, strafe);
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 2.0f; // 2m sobre el terreno
		if (isPointOutsideSphere(camara->getPos(), coordenadas)) vel += 0;
	}

	void RenderSkydome()
	{
		gradient += (1.0f / 1800.0f);
		if (gradient >= 1.0f) {
			gradient = 0.0;
		}
		counterDiaNoche += (1.0f / 60.0f);
		if (counterDiaNoche >= 60.0f)
			counterDiaNoche = 0;
		skydome->Update(camara->vista, camara->proyeccion);
		TurnOffDepth();
		skydome->Render(camara->posCam, counterDiaNoche, gradient);
		d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		TurnOnDepth();
	}

	void logicCollision()
	{
		if (vida <= 0) {
			OutputDebugString(L"Sin vidas\n");
			return;
		}

		bool ahoraDentroTorre = !isPointOutsideSphere(camara->getPos(), torre->getSphere(20.0f));

		if (ahoraDentroTorre && !dentroTorre)
		{
			OutputDebugString(L"Tocando la colisión\n");
			dentroTorre = true;
			vida--;

			switch (vida)
			{
			case 3:
				rutaVidaHud = L"Assets/GUI/vida3.png";
				break;
			case 2:
				rutaVidaHud = L"Assets/GUI/vida2.png";
				break;
			case 1:
				rutaVidaHud = L"Assets/GUI/vida1.png";
				break;
			}

			inicializarGUI(); // recarga la textura de vida
		}
		else if (!ahoraDentroTorre && dentroTorre)
		{
			// Restablece el estado para poder detectar la siguiente entrada
			dentroTorre = false;
		}
	}


	void RenderScene()
	{
		// Renderizar objetos ---------------------------------------------------------------------------------------------------

		terreno->Draw(camara->vista, camara->proyeccion);

		// Modelos

		arbolQuemado->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-90, 60), camara->posCam, 0.0f, 0.0f, 'A', 4);
		carreta->Draw(camara->vista, camara->proyeccion, terreno->Superficie(60, -90), camara->posCam, 0.0f, 0.0f, 'A', 4);
		casa1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(80, -110), camara->posCam, 0, 0, 'A', 1);
		casa2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-115, -115), camara->posCam, 0, 0, 'A', 3);
		castillo->Draw(camara->vista, camara->proyeccion, terreno->Superficie(90, 110), camara->posCam, 0.0f, 90.0f, 'Y', 2);
		castilloInterior->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 130) + 0.5f, camara->posCam, 0.0f, 90.0f, 'Y', 2);
		dragon->Draw(camara->vista, camara->proyeccion, terreno->Superficie(130, -40) + 5.0f, camara->posCam, 0, 0, 'A', 1);
		mesa->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-100, 100), camara->posCam, 0, 0, 'A', 4);
		pozo->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-50, -125), camara->posCam, 0, 0, 'A', 2);
		torre->Draw(camara->vista, camara->proyeccion, terreno->Superficie(120, -110) + 10.0f, camara->posCam, 0, 0, 'A', 1);
		vaca->Draw(camara->vista, camara->proyeccion, terreno->Superficie(vaca->getPosX(), vaca->getPosZ()), camara->posCam, 0, 0, 'A', 3);

		// Billboards

		billboard->DrawStatic(camara->vista, camara->proyeccion, camara->posCam, 0, 0, terreno->Superficie(0, 0), 5);

		// HUD
		HUD1->Draw(-0.75f, 0.75f);
		HUD2->Draw(0.6f, -0.75f);
		VIDA->Draw(-0.8f, -0.75f);

		textPt->DrawText(-0.5f, 0.9f, Puntos, 0.02f);
		textAt->DrawText(-0.45f, 0.7f, Puntos, 0.02f);
		textEn->DrawText(0.5f, -0.85f, Puntos, 0.02f);

		// ----------------------------------------------------------------------------------------------------------------------
	}

	void PresentFrame()
	{
		// Presentar el contenido
		swapChain->Present(1, 0);
	}

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	void Update(float deltaTime)
	{
		float prevX = camara->posCam.x;
		float prevZ = camara->posCam.z;

		camara->UpdateCam(vel * deltaTime, arriaba, izqder, strafe * deltaTime);



		// Actualizar posición Y basada en el terreno
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 2.0f;

		frameBillboard++;
		if (frameBillboard >= 32)
			frameBillboard = 0;
	}

	bool isPointInsideSphere(float* point, float* sphere) {
		float distance = sqrt(
			(point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[2] - sphere[2]) * (point[2] - sphere[2])
		);

		return distance < sphere[3];
	}

	bool isPointOutsideSphere(float* point, float* sphere) {
		float distance = sqrt(
			(point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[2] - sphere[2]) * (point[2] - sphere[2])
		);

		return distance > sphere[3];
	}

	bool Colision() {
		return isPointInsideSphere(camara->getPos(), vaca->getSphere(30.0f)) ||
			isPointInsideSphere(camara->getPos(), pozo->getSphere(30.0f));
	}

	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}

	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable = true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);

		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable = true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}

	void animacion()
	{
		uv1[0].u = .125;
		uv2[0].u = .125;
		uv3[0].u = 0;
		uv4[0].u = 0;

		uv1[0].v = .25;
		uv2[0].v = 0;
		uv3[0].v = 0;
		uv4[0].v = .25;


		for (int j = 0; j < 8; j++) {
			uv1[j].u = uv1[0].u + (j * .125);
			uv2[j].u = uv2[0].u + (j * .125);
			uv3[j].u = uv3[0].u + (j * .125);
			uv4[j].u = uv4[0].u + (j * .125);

			uv1[j].v = .25;
			uv2[j].v = 0;
			uv3[j].v = 0;
			uv4[j].v = .25;
		}
		for (int j = 0; j < 8; j++) {
			uv1[j + 8].u = uv1[0].u + (j * .125);
			uv2[j + 8].u = uv2[0].u + (j * .125);
			uv3[j + 8].u = uv3[0].u + (j * .125);
			uv4[j + 8].u = uv4[0].u + (j * .125);

			uv1[j + 8].v = .5;
			uv2[j + 8].v = .25;
			uv3[j + 8].v = .25;
			uv4[j + 8].v = .5;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 16].u = uv1[0].u + (j * .125);
			uv2[j + 16].u = uv2[0].u + (j * .125);
			uv3[j + 16].u = uv3[0].u + (j * .125);
			uv4[j + 16].u = uv4[0].u + (j * .125);

			uv1[j + 16].v = .75;
			uv2[j + 16].v = .5;
			uv3[j + 16].v = .5;
			uv4[j + 16].v = .75;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 24].u = uv1[0].u + (j * .125);
			uv2[j + 24].u = uv2[0].u + (j * .125);
			uv3[j + 24].u = uv3[0].u + (j * .125);
			uv4[j + 24].u = uv4[0].u + (j * .125);

			uv1[j + 24].v = 1;
			uv2[j + 24].v = .75;
			uv3[j + 24].v = .75;
			uv4[j + 24].v = 1;
		}
	}

};
#endif