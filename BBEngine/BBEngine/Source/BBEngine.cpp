#include "BBEngine.h"
#include "Matrix4x4.h"
#include "Logger/Logger.h"

#include "Utility/BBMemory.h"
#include "Thread/ThreadPool.h"
#include "FileLoaders/Models/GLTFParser.h"
#include "Vector4.h"

#include "System/FileHandler.h"

#include "GameLib/Components/Transform.h"
#include "GameLib/Components/MeshComponent.h"
#include "GameLib/Components/LightComponents/DirectionalLightComponent.h"
#include "GameLib/Components/LightComponents/PointLightComponent.h"
#include "GameLib/Components/LightComponents/SpotLightComponent.h"

#include <chrono>
#include <iostream>
#include <cstdint>
#include <stack>

#include <stb_image.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "UserInterface.h"

#include "Camera.h"
#include "FileLoaders/Image/BMPLoader.h"

namespace BBE
{
    TaskDesc* testdesc = nullptr;

    BBEngine::BBEngine()
        : m_Window(1600, 900, "BBWindow test")
    {
        BB_Log_Init("BBLogger", LOG_ALL, "logs/");
        m_ArenaAllocator.Init(BBE::PageSize);
        m_StackAllocator.Init(128 * BBE::MBSize);
        m_ThreadPool = BBNew(m_ArenaAllocator, BBE::ThreadPool)(8, 2);
    }

    BBEngine::~BBEngine()
    {
        BBFree(m_ArenaAllocator, m_ThreadPool);
    }

    int BBEngine::StartBBEngine()
    {
        Initialize();

        try
        {
            for (;;)
            {
                if (const auto ecode = BBWindow::ProcessMessages())
                    return *ecode;

                Update();
            }
        }
        catch (const BBException& e)
        {
            MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
        }
        catch (const std::exception& e)
        {
            MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
        }
        catch (...)
        {
            MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
        }
        return -1;
    }

    void BBEngine::Initialize()
    {
        GLTFParser parser;
        parser.Parse("Assets/Models/Sponza/Sponza/", "Sponza.gltf", &m_SponzaFile);
        parser.Parse("Assets/Models/Lantern/glTF/", "Lantern.gltf", &m_LanternFile);
        parser.Parse("Assets/Models/ToyCar/glTF/", "ToyCar.gltf", &m_CarFile);
        parser.Parse("Assets/Models/ABeautifulGame/glTF/", "ABeautifulGame.gltf", &m_ABeautifulGameFile);

        BBE:UI::InitializeUI(m_Graphics, &m_GameObjects);

        m_Window.m_Keyboard.EnableAutorepeat();

        m_DirectionalLight = DirectionalLight(
            Vector3(0.0f, -1.0f, 0.0f),
            Vector4(0.1f, 0.1f, 0.1f, 1.0f),
            Vector4(0.5f, 0.5f, 0.5f, 1.0f)
        );

        m_PointLights.Push_Back(PointLight(
            Vector3(-5.0f, 2.0f, 0.0f),
            Vector3(0.0f, 0.2f, 0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f),
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            1000.0f
        ));

        m_SpotLights.Push_Back(SpotLight(
            Vector3(0.0f, 2.0f, 0.0f),
            Vector3(-1.0f, 0.0f, 0.0f),
            5.f,
            Vector3(0.4f, 0.2f, 0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f),
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            1000.0f
        ));

        m_Graphics.CreateDirLightShadowBuffer(&m_DLTextureDepthStencilView);

        m_Graphics.CreatePointLightDepthCubeMapArray();
        for (uint32_t i = 0; i < m_PointLights.Size(); i++)
        {
            m_Graphics.CreatePointLightDepthCubeMap(m_PLTextureDepthStencilViews[i], i);
        }

        m_Graphics.CreateSpotLightDepthMapArray();
        for (uint32_t i = 0; i < m_SpotLights.Size(); i++)
        {
            m_Graphics.CreateSpotLightDepthTexture(&m_SLTextureDepthStencilViews[i], i);
        }
        
        m_PerFrameBuffer = PixelConstantBuffer<cbPerFrame>(m_Graphics);
        m_PerFrameBuffer.Bind(m_Graphics);

        m_ShadowMapCB = PixelConstantBuffer<ShadowMapCreation>(m_Graphics, 1, 1);
        m_ShadowMapCB.Bind(m_Graphics);

        m_LightMatrix = VertexConstantBuffer<vcbPerFrame>(m_Graphics, 1, 1);
        m_LightMatrix.Bind(m_Graphics);

        m_VertexShader = m_Graphics.CreateShader(ShaderType::VertexShader, "Assets/VertexShader.hlsl");
        m_PixelShader = m_Graphics.CreateShader(ShaderType::PixelShader, "Assets/PixelShader.hlsl");

        m_RTTVertexShader = m_Graphics.CreateShader(ShaderType::VertexShader, "Assets/VSDrawToTexture.hlsl");
        m_RTTPixelShader = m_Graphics.CreateShader(ShaderType::PixelShader, "Assets/PSDrawToTexture.hlsl");

        m_VSShadowMapShader = m_Graphics.CreateShader(ShaderType::VertexShader, "Assets/VSShadowMap.hlsl");
        m_PSShadowMapShader = m_Graphics.CreateShader(ShaderType::PixelShader, "Assets/PSShadowMap.hlsl");
        
        m_PSSpotLightShadowMapShader = m_Graphics.CreateShader(ShaderType::PixelShader, "Assets/PSShadowMap.hlsl", "SpotLightPS");

        //Models
        Model* Sponza = BBNew(m_StackAllocator, Model)(m_Graphics, "Sponza", &m_SponzaFile, m_VertexShader, m_PixelShader);
        m_Models.push_back(Sponza);
        Model* lantern = BBNew(m_StackAllocator, Model)(m_Graphics, "Lantern", &m_LanternFile, m_VertexShader, m_PixelShader);
        m_Models.push_back(lantern);
        Model* car = BBNew(m_StackAllocator, Model)(m_Graphics, "Car", &m_CarFile, m_VertexShader, m_PixelShader);
        m_Models.push_back(car);
        //Model* aBeautifulGame = BBNew(m_StackAllocator, Model)(m_Graphics, &m_ABeautifulGameFile, m_VertexShader, m_PixelShader);
        //m_Models.push_back(aBeautifulGame);

        //Skybox
        m_Skybox = BBNew(m_StackAllocator, Skybox)(m_Graphics);

        //Objects
        int XSize = 2, YSize = 2;
        for (size_t i = 0; i < XSize; i++) {
            for (size_t y = 0; y < YSize; y++) {
                BBObject* obj = BBNew(m_StackAllocator, BBObject)("Sponza");
                Transform* sponzaTransform = BBNew(m_StackAllocator, Transform)(Vector3(i * 50, 0, y * 50));
                MeshComponent* sponzaMesh = BBNew(m_StackAllocator, MeshComponent)(m_Graphics, Sponza, sponzaTransform);
                obj->AddComponent(sponzaTransform);
                obj->AddComponent(sponzaMesh);
                m_GameObjects.push_back(obj);
            }
        }

        BBObject* lanternBBObj = BBNew(m_StackAllocator, BBObject)("Lantern");
        Transform* lanternTransform = BBNew(m_StackAllocator, Transform)(Vector3(-3, 0, 0), Vector3(0, 0, 0), Vector3(0.2f, 0.2f, 0.2f));
        MeshComponent* lanternMesh = BBNew(m_StackAllocator, MeshComponent)(m_Graphics, lantern, lanternTransform);
        lanternBBObj->AddComponent(lanternTransform);
        lanternBBObj->AddComponent(lanternMesh);
        m_GameObjects.push_back(lanternBBObj);

        BBObject* carObject = BBNew(m_StackAllocator, BBObject)("Car");
        Transform* carTransform = BBNew(m_StackAllocator, Transform)(Vector3(0, 3, 0), Vector3(0, 0, 0), Vector3(0.2f, 0.2f, 0.2f));
        carObject->AddComponent(carTransform);
        MeshComponent* carMesh = BBNew(m_StackAllocator, MeshComponent)(m_Graphics, car, carTransform);
        carObject->AddComponent(carMesh);
        m_GameObjects.push_back(carObject);

        //Lights
        BBObject* spotLightObject = BBNew(m_StackAllocator, BBObject)("SpotLight");
        Transform* spotLightTransform = BBNew(m_StackAllocator, Transform)(Vector3(-3, 2, 0), Vector3(0, 0, 0), Vector3(0.2f, 0.2f, 0.2f));
        spotLightObject->AddComponent(spotLightTransform);
        SpotlightComponent* spotLightComponent = BBNew(m_StackAllocator, SpotlightComponent)(&m_SpotLights[0], spotLightTransform);
        spotLightObject->AddComponent(spotLightComponent);
        m_GameObjects.push_back(spotLightObject);

        BBObject* dirLightObject = BBNew(m_StackAllocator, BBObject)("DirectionalLight");
        Transform* dirlightTransform = BBNew(m_StackAllocator, Transform)(Vector3(-20.0f, 50.0f, 0), Vector3(0, 0, 0), Vector3(0.2f, 0.2f, 0.2f));
        dirLightObject->AddComponent(dirlightTransform);
        DirectionalLightComponent* directionalLightComponent = BBNew(m_StackAllocator, DirectionalLightComponent)(&m_DirectionalLight, dirlightTransform);
        dirLightObject->AddComponent(directionalLightComponent);
        m_GameObjects.push_back(dirLightObject);

        BBObject* pointLightObject = BBNew(m_StackAllocator, BBObject)("PointLight");
        Transform* pointlightTransform = BBNew(m_StackAllocator, Transform)(Vector3(-5.0f, 2.0f, 0.0f), Vector3(0, 0, 0), Vector3(0.2f, 0.2f, 0.2f));
        pointLightObject->AddComponent(pointlightTransform);
        PointLightComponent* pointLightComponent = BBNew(m_StackAllocator, PointLightComponent)(&m_PointLights[0], pointlightTransform);
        pointLightObject->AddComponent(pointLightComponent);
        m_GameObjects.push_back(pointLightObject);

        //Cameras
        m_Cam1.SetPosition(DirectX::XMVectorSet(0, 0, 0, 0));
        m_Cam1.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 100.f));

        m_Cam2.SetPosition(DirectX::XMVectorSet(0, 0, 0, 0));
        m_Cam2.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 100.f));

        m_Cam2.SetViewPort(1024, 1024);
        m_Graphics.SetCamera(&m_Cam1);
    }

    bool show_demo_window = true;
    float incr = 0;
    void BBEngine::Update()
    {
        m_Graphics.SetGameViewRenderTarget();
        m_Graphics.ClearBuffer(0.07f, 0.0f, 0.012f);

        CheckInput();
        m_Skybox->Draw(m_Graphics);

        cbPerFrame FrameConstantBuffer;
        FrameConstantBuffer.directionalLight = m_DirectionalLight;
        CalculateLightShadowMapDirectionalLight(m_GameObjects, m_VSShadowMapShader, m_PSSpotLightShadowMapShader);

        for (uint32_t i = 0; i < m_SpotLights.Size(); i++) {
            FrameConstantBuffer.spotlights[i] = m_SpotLights[i];
            CalculateLightShadowMapSpotLight(m_GameObjects, m_VSShadowMapShader, m_PSSpotLightShadowMapShader, i);
        }
        
        for (uint32_t i = 0; i < m_PointLights.Size(); i++) {
            FrameConstantBuffer.pointlights[i] = m_PointLights[i];
            CalculateLightShadowMapPointLight(m_GameObjects, m_VSShadowMapShader, m_PSShadowMapShader, m_PointLights[i], i);
        }

        m_PerFrameBuffer.Update(m_Graphics, FrameConstantBuffer);

        for (size_t i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects[i]->Update(m_Graphics);
        }
       
        m_Graphics.BindDepthSampler();
        m_Graphics.BindDepthTexture(m_Graphics.GetPointLightDepthCubeArrayRSV(), 2, 1);
        m_Graphics.BindDepthTexture(m_Graphics.GetSpotLightDepthMapArrayRSV(), 3, 1);
        m_Graphics.BindDepthTexture(m_Graphics.GetDirectionLightDepthMapRSV(), 4, 1);

        for (size_t i = 0; i < m_GameObjects.size(); i++) {
            m_Graphics.BindShader(ShaderType::VertexShader, m_VertexShader);
            m_Graphics.BindShader(ShaderType::PixelShader, m_PixelShader);
            m_GameObjects[i]->Draw(m_Graphics);
        }

        m_Graphics.UnbindSRV(1);
        m_Graphics.UnbindSRV(2);
        m_Graphics.UnbindSRV(3);
        m_Graphics.UnbindSRV(4);

        m_Graphics.ResetRenderTarget();
       
        BBE:UI::UpdateUI(m_Graphics);

        m_Graphics.EndFrame();
    }

    void BBEngine::DrawAllModels()
    {
        for (uint32_t i = 0; i < m_Models.size(); i++)
        {
            m_Graphics.BindShader(ShaderType::VertexShader, m_VertexShader);
            m_Graphics.BindShader(ShaderType::PixelShader, m_PixelShader);
            m_Models[i]->Draw(m_Graphics);
        }
    }

    void BBEngine::CalculateLightShadowMapDirectionalLight(std::vector<BBObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader)
    {
        float cx = -20.0f, cy = 50.0f, cz = 0.0f;
        Vector3 FakePos = Vector3(cx, cy, cz);
        Vector3 focusPoint = Vector3(0, 0, 0);

        float length = sqrt(cx * cx + cy * cy + cz * cz);
        m_DirectionalLight.direction = Vector3(cx / length, cy / length, cz / length);

        DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH(
            DirectX::XMVectorSet(FakePos.x, FakePos.y, FakePos.z, 0),
            DirectX::XMVectorSet(focusPoint.x, focusPoint.y, focusPoint.z, 1.0f),
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
        );

        DirectX::XMMATRIX projection = DirectX::XMMatrixOrthographicLH(150.f, 150.f, 0.5f, 500.f);
        DirectX::XMMATRIX oldProjection = m_Cam2.GetProjection();
        m_Cam2.SetProjection(projection);
        m_Cam2.SetViewPort(8192, 8192);

        m_DirectionalLight.lightView = DirectX::XMMatrixTranspose(lightView * projection);

        m_Cam2.m_ViewMatrix = lightView;
        m_Graphics.SetCamera(&m_Cam2);
        m_Graphics.SetDepthStencilTarget(m_DLTextureDepthStencilView);
    
        for (size_t i = 0; i < a_GameObjects.size(); i++)
        {
            BBObject* obj = a_GameObjects[i];
            //obj->GetModel()->SetCurrentShader(a_VSShadowMapShader, a_PSShadowMapShader);
            m_Graphics.BindShader(ShaderType::VertexShader, a_VSShadowMapShader);
            m_Graphics.BindShader(ShaderType::PixelShader, a_PSShadowMapShader);
            obj->Draw(m_Graphics);
            //obj->GetModel()->ResetShaders();
        }

        m_Cam2.SetProjection(oldProjection);
        m_Cam2.SetViewPort(1024, 1024);
        m_Graphics.SetGameViewRenderTarget();
        m_Graphics.SetCamera(&m_Cam1);

    }

    void BBEngine::CalculateLightShadowMapSpotLight(std::vector<BBObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader, uint32_t a_Index)
    {
        Vector3 focusPoint = m_SpotLights[a_Index].position + m_SpotLights[a_Index].direction;
        DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH(
            DirectX::XMVectorSet(m_SpotLights[a_Index].position.x, m_SpotLights[a_Index].position.y, m_SpotLights[a_Index].position.z, 0),
            DirectX::XMVectorSet(focusPoint.x, focusPoint.y, focusPoint.z, 1.0f),
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
        );


        m_Cam2.m_ViewMatrix = lightView;
        m_Cam2.SetViewPort(1024, 1024);
        m_Graphics.SetCamera(&m_Cam2);
        m_SpotLights[a_Index].lightView = DirectX::XMMatrixTranspose(lightView * m_Graphics.GetCamera()->GetProjection());
        m_Graphics.SetDepthStencilTarget(m_SLTextureDepthStencilViews[a_Index]);

        for (size_t i = 0; i < a_GameObjects.size(); i++)
        {
            BBObject* obj = a_GameObjects[i];
            //obj->GetModel()->SetCurrentShader(a_VSShadowMapShader, a_PSShadowMapShader);
            m_Graphics.BindShader(ShaderType::VertexShader, a_VSShadowMapShader);
            m_Graphics.BindShader(ShaderType::PixelShader, a_PSShadowMapShader);
            obj->Draw(m_Graphics);
            //obj->GetModel()->ResetShaders();
        }

        m_Graphics.SetGameViewRenderTarget();
        m_Graphics.SetCamera(&m_Cam1);
    }

    void BBEngine::CalculateLightShadowMapPointLight(std::vector<BBObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader, PointLight a_Spotlight, uint32_t a_Index)
    {
        Vector3 viewDirections[6] = { 
            { 1.0f, 0.0f, 0.0f },    // Right
            { -1.0f, 0.0f, 0.0f },   // Left
            { 0.0f, 1.0f, 0.001f },   // top  
            { 0.0f, -1.0f, 0.001f },   // Bottom
            { 0.0f, 0.0f, 1.0f },   // front
            { 0.0f, 0.0f, -1.0f },   // back
        };

        m_Graphics.SetCamera(&m_Cam2);
        
        m_ShadowMapCBBuffer.index = a_Index;
        m_ShadowMapCB.Update(m_Graphics, m_ShadowMapCBBuffer);

        for (uint32_t depthStencilIndex = 0; depthStencilIndex < CUBEMAP_SIZE; depthStencilIndex++)
        {
            Vector3 focusPoint = a_Spotlight.position + viewDirections[depthStencilIndex];
            DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH(
                DirectX::XMVectorSet(a_Spotlight.position.x, a_Spotlight.position.y, a_Spotlight.position.z, 0),
                DirectX::XMVectorSet(focusPoint.x, focusPoint.y, focusPoint.z, 1.0f),
                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
            );

            m_Cam2.m_ViewMatrix = lightView;
            m_Graphics.SetDepthStencilTarget(m_PLTextureDepthStencilViews[a_Index][depthStencilIndex]);
            m_Graphics.SetCamera(&m_Cam2);

            for (size_t gameObjIndex = 0; gameObjIndex < a_GameObjects.size(); gameObjIndex++)
            {
                BBObject* obj = a_GameObjects[gameObjIndex];
                //obj->GetModel()->SetCurrentShader(a_VSShadowMapShader, a_PSShadowMapShader);
                m_Graphics.BindShader(ShaderType::VertexShader, a_VSShadowMapShader);
                m_Graphics.BindShader(ShaderType::PixelShader, a_PSShadowMapShader);
                obj->Draw(m_Graphics);
                //obj->GetModel()->ResetShaders();
            }

        }

        m_Graphics.SetGameViewRenderTarget();
        m_Graphics.SetCamera(&m_Cam1);
    }

    void BBEngine::CheckInput()
    {
        Camera* cam = m_Graphics.GetCamera();
        Keyboard& kBoard = m_Window.m_Keyboard;

        if (kBoard.KeyIsPressed('W'))
        {
            cam->camForwardMove += 0.1f;
        }
        if (kBoard.KeyIsPressed('S'))
        {
            cam->camForwardMove += -0.1f;
        }
        if (kBoard.KeyIsPressed('A'))
        {
            cam->camRightMove += -0.1f;
        }
        if (kBoard.KeyIsPressed('D'))
        {
            cam->camRightMove += 0.1f;
        }
        if (kBoard.KeyIsPressed(VK_NUMPAD1))
        {
            cam->camUpMove = 0.1f;
        }
        if (kBoard.KeyIsPressed(VK_NUMPAD0))
        {
            cam->camUpMove = -0.1f;
        }
        if (kBoard.KeyIsPressed(VK_LEFT))
        {
            cam->camYaw += -0.025f;
        }
        if (kBoard.KeyIsPressed(VK_RIGHT))
        {
            cam->camYaw += 0.025f;
        }
        if (kBoard.KeyIsPressed(VK_UP))
        {
            cam->camPitch += -0.025f;
        }
        if (kBoard.KeyIsPressed(VK_DOWN))
        {
            cam->camPitch += 0.025f;
        }

        if (kBoard.KeyIsPressed('1')) {
            m_Graphics.SetCamera(&m_Cam1);
        }

        if (kBoard.KeyIsPressed('2')) {
            m_Graphics.SetCamera(&m_Cam2);
        }

        cam->Update();
    }
}