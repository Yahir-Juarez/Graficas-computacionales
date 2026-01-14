#include <SFML/Graphics.hpp>
#include "include\imgui.h"
#include "include\imgui-SFML.h"

#include <Windows.h>
#include <codecvt>

#include "Image.h"
#include "ImageProceUI_App.h"

#include <cstddef>
#include <cstdint>

const float margenX = .80;
const float margenY = .50;

const ImVec2 buttonSize2 = ImVec2(146, 80);
const ImVec2 buttonSize = ImVec2(299, 80);
const ImVec2 buttonSize3 = ImVec2(200, 80);
sf::RenderWindow window;

Image image;
Image imageExport;

Image imageRaster;
Image imageOptionalRaster;
sf::Texture tex;
sf::Texture texRaster;
sf::Texture texImageExport;

sf::Image testImage;
sf::Image testImageRaster;

sf::Image testImageExport;

ImFont* font;



float lsR2 = 0.33f;
float lsG2 = 0.60f;
float lsB2 = 0.03f;

bool bShowRasterWindow = false;

bool bApplyPostProcess = false;

void
renderWindowImport();

void
renderWindowImportRaster();

void
renderWindowExport();

void
renderWindowTools();

void
renderWindowToolsRaster();

std::string
openFileExplorer();

void
exportImage();

std::string
wcharToString2(const wchar_t* wideString);

sf::Image
createImageSFML(Image& baseImage);

sf::Vector2u
resizeImageButton(sf::Vector2u sizeImage);

sf::Vector2u
getPosImageButton(sf::Vector2u sizeImage);

void
setTextureInTex(Image& image, sf::Image& imageSFML, sf::Texture& texImage);
int main()
{
  //resizeImageButton(sf::Vector2u(700, 1500));
  //g_CodecMan.AddCodec(new BMPCodec());
  sf::VideoMode size = sf::VideoMode(sf::Vector2u(1785, 900));

  //image.CreateFromImageFile("C:/Users/super/OneDrive/Escritorio/Graficas computacionales/Graficas computacionales/Yoda.bmp");
  window = sf::RenderWindow(size, "Image Processor");
  window.setFramerateLimit(60);

  sf::IntRect rect(sf::Vector2i(0, 0), sf::Vector2i(image.GetWidth(), image.GetHeight()));

  testImage = sf::Image( Path("Resources/Import.png"));
  testImageExport = sf::Image(Path("Resources/Export.png"));
  //testImage = createImageSFML(image);

  tex = sf::Texture(testImage);
  texRaster = sf::Texture(testImage);
  texImageExport = sf::Texture(testImageExport);

  // Inicializar ImGui-SFML
  ImGui::SFML::Init(window);

  // Variables UI
  char inputText[128] = "Hola mi compa";
  float sliderValue = 0.0f;
  bool showWindow = true;

  sf::Clock deltaClock;

  //ImGui::CreateContext();

  //ImGuiIO& io = ImGui::GetIO();

  // Crear una fuente de tamaño 24 (por ejemplo)
  //font = io.Fonts->AddFontFromFileTTF("C:/Users/super/OneDrive/Escritorio/Spine Pro v3.8.75 (WIN)/Setup/launcher/fonts/NotoSans-Regular.ttf", 24.0f);


  while (window.isOpen())
  {
    std::optional<sf::Event> wndEvent;
    while (wndEvent = window.pollEvent()) {
      if (const auto* wndCommand = wndEvent.value().getIf<sf::Event::WndCommand>())
      {
        //WindowProc(hwnd, WM_COMMAND, wndCommand->wParam, wndCommand->lParam);
      }
      if (wndEvent.value().getIf<sf::Event::Closed>()) {
        window.close();
      }
      ImGui::SFML::ProcessEvent(window, *wndEvent);
    }

    // Actualizar ImGui
    ImGui::SFML::Update(window, deltaClock.restart());

    // --- Crear ventana ImGui ---
    if (showWindow)
    {
      ImGui::Begin("Mi Ventana ImGui", &showWindow);

      ImGui::Text("Ejemplo SFML + ImGui");
      ImGui::InputText("Escribe algo", inputText, IM_ARRAYSIZE(inputText));

      ImGui::SliderFloat("Valor", &sliderValue, 0.0f, 100.0f);

      if (ImGui::Button("Presioname"))
      {
        printf("Boton presionado! Texto: %s\n", inputText);
      }

      ImGui::End();
    }
    renderWindowImport();

    if (!bShowRasterWindow) {
      renderWindowExport();
      renderWindowTools();
    }
    else {
      renderWindowImportRaster();
      renderWindowToolsRaster();
    }
    // Render normal
    window.clear(sf::Color(50, 50, 50));

    // Render ImGui encima
    ImGui::SFML::Render(window);

    window.display();
  }

  // Limpieza final
  ImGui::SFML::Shutdown();

  return 0;
}

void
renderWindowImport()
{
  ImGui::SetNextWindowPos(ImVec2(15, 15));
  ImGui::SetNextWindowSize(ImVec2(700, 875), ImGuiCond_Always);

  sf::Vector2u sizeImage = tex.getSize();

  ImGui::Begin("Import BMP");
  sf::Vector2u sizeImage2 = resizeImageButton(sizeImage);
  sf::Vector2u size2 = getPosImageButton(sizeImage2);
  ImGui::SetCursorPos(ImVec2(size2.x, size2.y));

  // Cambiar la fuente
  //ImGui::PushFont(font);

  if (ImGui::ImageButton("ImportImage", tex, sf::Vector2f(sizeImage2))) {
    std::string file = openFileExplorer();
    image.CreateFromImageFile(file);

    testImage = createImageSFML(image);

    tex = sf::Texture(testImage);
  }

  //ImGui::SetCursorPos(ImVec2(0, 666));

  //ImGui::Separator();

  ImGui::SetCursorPos(ImVec2(250, 770));
  if (ImGui::Button("Import", buttonSize3)) {
    std::string file = openFileExplorer();
    image.CreateFromImageFile(file);

    testImage = createImageSFML(image);

    tex = sf::Texture(testImage);
  }
  ImGui::SetCursorPos(ImVec2(5, 850));
  ImGui::Checkbox("Automatic P-P Application", &bApplyPostProcess);
  ImGui::End();
}


void
renderWindowExport()
{
  ImGui::SetNextWindowPos(ImVec2(730, 15));
  ImGui::SetNextWindowSize(ImVec2(700, 875), ImGuiCond_Always);

  sf::Vector2u sizeImage = texImageExport.getSize();

  ImGui::Begin("Export BMP");
  sf::Vector2u sizeImage2 = resizeImageButton(sizeImage);
  sf::Vector2u size2 = getPosImageButton(sizeImage2);
  ImGui::SetCursorPos(ImVec2(size2.x, size2.y));


  if (ImGui::ImageButton("ExportImage", texImageExport, sf::Vector2f(sizeImage2))) {
    exportImage();
  }

  ImGui::SetCursorPos(ImVec2(250, 770));
  if (ImGui::Button("Export", buttonSize3)) {
    exportImage();
  }
  ImGui::End();
}

std::string
wcharToString2(const wchar_t* wideString)
{
  int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideString, -1, NULL, 0, NULL, NULL);

  std::string result(sizeNeeded, 0);

  WideCharToMultiByte(CP_UTF8, 0, wideString, -1, &result[0], sizeNeeded, NULL, NULL);
  return result;
}

#include <random>
#include <cmath>

int Random2(int min, int max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(min, max);
  return dis(gen);
}

void
renderWindowTools()
{
  ImGui::SetNextWindowPos(ImVec2(1445, 15));
  ImGui::SetNextWindowSize(ImVec2(325, 875), ImGuiCond_Always);
  ImGui::Begin("Tools");

  static int levelBlur = 1;

  ImGui::Separator();
  ImGui::Text("SIZE");
  ImGui::Separator();

  static float valueSize = 1.0f;
  ImGui::Text("Image Size");
  ImGui::SetNextItemWidth(300);
  ImGui::InputFloat("##Image Size", &valueSize);
  if (ImGui::Button("Resize Image", buttonSize)) {
    imageExport.scaleImg(valueSize, image);

    setTextureInTex(imageExport, testImageExport, texImageExport);

    if (bApplyPostProcess) {
      tex = texImageExport;
      image = imageExport;
    }
  }

  ImGui::Separator();
  ImGui::Text("ROTATION");
  ImGui::Separator();

  static float rotateAngle = 0.0f;

  ImGui::Text("Degrees of rotation");
  ImGui::SetNextItemWidth(300);
  ImGui::SliderFloat("##Degrees of rotation", &rotateAngle, 0.0f, 360.0f);

  if (ImGui::Button("Roatate Image", buttonSize)) {
    imageExport.rotate(rotateAngle, image);

    setTextureInTex(imageExport, testImageExport, texImageExport);

    if (bApplyPostProcess) {
      tex = texImageExport;
      image = imageExport;
    }
  }

  ImGui::Separator();
  ImGui::Text("BIT BLIT");
  ImGui::Separator();

  static int CountBitBlit = 100.0f;
  static sf::Vector2 sizeImage(1920, 1080);
  ImGui::Text("Size Image");
  ImGui::SetNextItemWidth(300);
  ImGui::InputInt2("##Size Image", &sizeImage.x);
  if (ImGui::Button("Bit Blit", buttonSize)) {
    imageExport.CreateImage(sizeImage.x, sizeImage.y, image.m_bpp);

    for (int i = 0; i < CountBitBlit; i++)
    {
      imageExport.bitBlt(image, Random2(sizeImage.x * -1, sizeImage.x), Random2(sizeImage.y * -1, sizeImage.y),
        Random2((0), image.m_width)), Random2((0), image.m_height),
        Random2((0), image.m_width), Random2((0), image.m_height);
    }

    setTextureInTex(imageExport, testImageExport, texImageExport);

    if (bApplyPostProcess) {
      tex = texImageExport;
      image = imageExport;
    }
  }
  ImGui::Text("Count Bit Blits");
  ImGui::SetNextItemWidth(300);
  ImGui::InputInt("##Count Bit Blits", &CountBitBlit);

  ImGui::Separator();
  ImGui::Text("FILTERS");
  ImGui::Separator();

  ImGui::Text("Blur Level");
  ImGui::SetNextItemWidth(300);
  ImGui::SliderInt("##Blur Level", &levelBlur, 1, 20);

  if (ImGui::Button("Blur", buttonSize)) {
    imageExport = ImageProceUI_APP::resolveImageKebler(image, KEBLERS::kBlur, levelBlur);

    setTextureInTex(imageExport, testImageExport, texImageExport);

    if (bApplyPostProcess) {
      tex = texImageExport;
      image = imageExport;
    }
  }


  if (ImGui::Button("Sampler Bilineal", buttonSize2)) {

    imageExport.CreateImage(image.m_width, image.m_height, image.m_bpp);
    for (int iY = 0; iY < image.m_height; iY++)
    {
      for (int iX = 0; iX < image.m_width; iX++)
      {
        float siU = (1.0f / image.m_width) * iX;
        float siV = (1.0f / image.m_height) * iY;
        Color color(image.SampleBilineal(siU, siV));
        imageExport.SetPixel(iX, iY, color);
      }
    }

    setTextureInTex(imageExport, testImageExport, texImageExport);

    if (bApplyPostProcess) {
      tex = texImageExport;
      image = imageExport;
    }
  }

  ImGui::SameLine();

  if (ImGui::Button("Gray Scale", buttonSize2)) {

    imageExport = image.ProcessImage(ImageProceUI_APP::greyScale);

    setTextureInTex(imageExport, testImageExport, texImageExport);

    if (bApplyPostProcess) {
      tex = texImageExport;
      image = imageExport;
    }
  }

  ImGui::Separator();
  ImGui::Text("EDGE DETECTOR");
  ImGui::Separator();

  const char* items[] = { "Outline", "Left Sobel", "Top Sobel", "Sum Sobel", "Left Prewwit", "Top Prewwit" };

  static int32 current_item = 0;

  ImGui::Text("Types Kernels");
  ImGui::SetNextItemWidth(300);
  if (ImGui::BeginCombo("##Types Kernels", items[current_item])) {
    for (int32 n = 0; n < IM_ARRAYSIZE(items); ++n) {
      const bool is_selected = (current_item == n);

      if (ImGui::Selectable(items[n], is_selected)) {
        current_item = n;
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  if (ImGui::Button("Apply Kernerl", buttonSize)) {
    imageExport = ImageProceUI_APP::resolveImageKebler(image, static_cast<KEBLERS::E>(current_item + 1));

    //imageExport = image.ProcessImage(ImageProceUI_APP::greyScale);

    setTextureInTex(imageExport, testImageExport, texImageExport);

    if (bApplyPostProcess) {
      tex = texImageExport;
      image = imageExport;
    }
  }

  ImGui::Separator();
  ImGui::Text("RASTER");
  ImGui::Separator();

  static float rasterPosX = 0.0f;
  static float rasterPosY = 0.0f;
  static float rotateAngleRaster = 0.0f;

  ImGui::Text("Raster Pos Image X/Y");
  ImGui::SetNextItemWidth(300);
  ImGui::SliderFloat("##Raster Pos Image X", &rasterPosX, 0.0f, image.m_width);
  ImGui::SetNextItemWidth(300);
  ImGui::SliderFloat("##Raster Pos Image Y", &rasterPosY, 0.0f, image.m_height);
  if (ImGui::Button("Raster Image", buttonSize2)) {
    imageExport = image;
    imageExport.Raster(imageRaster, rasterPosX, rasterPosY);

    setTextureInTex(imageExport, testImageExport, texImageExport);

    if (bApplyPostProcess) {
      tex = texImageExport;
      image = imageExport;
    }
  }

  ImGui::SameLine();

  ImGui::Checkbox("Show Raster Tools", &bShowRasterWindow);

  ImGui::End();
}

std::string
openFileExplorer()
{
  OPENFILENAME ofn;       // Estructura para el diálogo
  wchar_t szFile[260] = { 0 };
  HWND hwnd = window.getNativeHandle();
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  //std::wstring filePathW(ofn.lpstrFile);
  //std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  //std::string filePathA = converter.to_bytes(filePathW);

  //g_pApp->loadImage(filePathA)
  // Abrir el cuadro de diálogo
  if (GetOpenFileName(&ofn) == TRUE) {
    std::string path = wcharToString2(szFile);
    std::string newPath;
    for (int i = 0; i < path.size(); i++) {
      if (path[i] == 92) {
        newPath.push_back('/');
      }
      else if (path[i] == '\0')
      {

      }
      else
      {
        newPath.push_back(path[i]);
      }
    }
    return newPath;
    //g_pApp2->loadImage(newPath);
    //MessageBox(hwnd, ofn.lpstrFile, L"Selected File", MB_OK);
  }
}

sf::Image
createImageSFML(Image& baseImage)
{
  std::vector<std::uint8_t> pixelRGBA;

  if (baseImage.m_width * baseImage.m_height * 4 != baseImage.m_pixelData.size()) {

    pixelRGBA.reserve(baseImage.m_pixelData.size() * 4 / 3);

    for (size_t i = 0; i < baseImage.m_pixelData.size(); i += 3)
    {
      std::uint8_t r = std::to_integer<std::uint8_t>(baseImage.m_pixelData[i]);
      std::uint8_t g = std::to_integer<std::uint8_t>(baseImage.m_pixelData[i + 1]);
      std::uint8_t b = std::to_integer<std::uint8_t>(baseImage.m_pixelData[i + 2]);

      pixelRGBA.push_back(b);
      pixelRGBA.push_back(g);
      pixelRGBA.push_back(r);
      pixelRGBA.push_back(255);
    }
  }
  else {
    for (size_t i = 0; i < baseImage.m_pixelData.size(); i++) {
      pixelRGBA.push_back(std::to_integer<std::uint8_t>(baseImage.m_pixelData[i]));
    }
  }

  return sf::Image(sf::Vector2u(baseImage.m_width, baseImage.m_height), pixelRGBA.data());
}

sf::Vector2u
resizeImageButton(sf::Vector2u sizeImage)
{
  if (sizeImage.x > 650 || sizeImage.y > 740) {
    sf::Vector2u newSizeImage;
    if (sizeImage.x > 650) {
      float resize = (static_cast<float>(sizeImage.x) - 650) / static_cast<float>(sizeImage.x);
      resize = 1 - resize;
      newSizeImage = sf::Vector2u(sizeImage.x * resize, sizeImage.y * resize);
    }
    else if (sizeImage.y > 740) {
      float resize = (static_cast<float>(sizeImage.y - 740)) / static_cast<float>(sizeImage.y);
      resize = 1 - resize;
      newSizeImage = sf::Vector2u(sizeImage.x * resize, sizeImage.y * resize);
    }
    return resizeImageButton(newSizeImage);
  }
  return sizeImage;
}

sf::Vector2u
getPosImageButton(sf::Vector2u sizeImage)
{
  return sf::Vector2u(350 - (sizeImage.x / 2), 385 - (sizeImage.y / 2));
}

void
renderWindowImportRaster()
{
  ImGui::SetNextWindowPos(ImVec2(730, 15));
  ImGui::SetNextWindowSize(ImVec2(700, 875), ImGuiCond_Always);

  sf::Vector2u sizeImage = texRaster.getSize();

  ImGui::Begin("Import Raster BMP");
  sf::Vector2u sizeImage2 = resizeImageButton(sizeImage);
  sf::Vector2u size2 = getPosImageButton(sizeImage2);
  ImGui::SetCursorPos(ImVec2(size2.x, size2.y));

  // Cambiar la fuente
  //ImGui::PushFont(font);

  if (ImGui::ImageButton("ImportImage", texRaster, sf::Vector2f(sizeImage2))) {
    std::string file = openFileExplorer();
    imageRaster.CreateFromImageFile(file);

    testImageRaster = createImageSFML(imageRaster);

    texRaster = sf::Texture(testImageRaster);
  }

  //ImGui::SetCursorPos(ImVec2(0, 666));

  //ImGui::Separator();

  ImGui::SetCursorPos(ImVec2(250, 770));
  if (ImGui::Button("Import Raster", buttonSize3)) {
    std::string file = openFileExplorer();
    imageRaster.CreateFromImageFile(file);

    testImageRaster = createImageSFML(imageRaster);

    texRaster = sf::Texture(testImageRaster);
  }
  ImGui::End();
}

void
renderWindowToolsRaster()
{
  ImGui::SetNextWindowPos(ImVec2(1445, 15));
  ImGui::SetNextWindowSize(ImVec2(325, 875), ImGuiCond_Always);
  ImGui::Begin("Tools Raster");

  ImGui::Separator();
  ImGui::Text("RASTER");
  ImGui::Separator();

  ImGui::Checkbox("Hide raster tools", &bShowRasterWindow);

  ImGui::Separator();
  ImGui::Text("SIZE");
  ImGui::Separator();

  static float valueSize = 1.0f;
  ImGui::Text("Image Size");
  ImGui::SetNextItemWidth(300);
  ImGui::InputFloat("##Image Size", &valueSize);

  if (ImGui::Button("Resize Image", buttonSize)) {
    imageOptionalRaster.scaleImg(valueSize, imageRaster);

    setTextureInTex(imageOptionalRaster, testImageRaster, texRaster);

    imageRaster = imageOptionalRaster;
  }

  ImGui::Separator();
  ImGui::Text("ROTATION");
  ImGui::Separator();

  static float rotateAngle = 0.0f;

  ImGui::Text("Degrees of rotation");
  ImGui::SetNextItemWidth(300);
  ImGui::SliderFloat("##Degrees of rotation", &rotateAngle, 0.0f, 360.0f);

  if (ImGui::Button("Roatate Image", buttonSize)) {
    imageOptionalRaster.rotate(rotateAngle, imageRaster);

    setTextureInTex(imageOptionalRaster, testImageRaster, texRaster);

    imageRaster = imageOptionalRaster;
  }

  ImGui::Separator();
  ImGui::Text("BIT BLIT");
  ImGui::Separator();

  static int CountBitBlit = 100.0f;
  static sf::Vector2 sizeImage(1920, 1080);
  ImGui::Text("Size Image");
  ImGui::SetNextItemWidth(300);
  ImGui::InputInt2("##Size Image", &sizeImage.x);

  if (ImGui::Button("Bit Blit", buttonSize)) {
    imageOptionalRaster.CreateImage(sizeImage.x, sizeImage.y, image.m_bpp);

    for (int i = 0; i < CountBitBlit; i++)
    {
      imageOptionalRaster.bitBlt(imageRaster, Random2(sizeImage.x * -1, sizeImage.x), Random2(sizeImage.y * -1, sizeImage.y),
        Random2((0), imageRaster.m_width)), Random2((0), imageRaster.m_height),
        Random2((0), imageRaster.m_width), Random2((0), imageRaster.m_height);
    }

    setTextureInTex(imageOptionalRaster, testImageRaster, texRaster);

    imageRaster = imageOptionalRaster;
  }
  ImGui::Text("Count Bit Blits");
  ImGui::SetNextItemWidth(300);
  ImGui::InputInt("##Count Bit Blits", &CountBitBlit);

  ImGui::Separator();
  ImGui::Text("FILTERS");
  ImGui::Separator();

  static int levelBlur = 1;
  ImGui::Text("Blur Level");
  ImGui::SetNextItemWidth(300);
  ImGui::SliderInt("##Blur Level", &levelBlur, 1, 20);

  if (ImGui::Button("Blur", buttonSize)) {
    imageRaster = imageRaster.ProcessImageColor(KeblerImage::blur);

    for (int i = 1; i < levelBlur; i++) {
      imageRaster = imageRaster.ProcessImageColor(KeblerImage::blur);
    }

    testImageRaster = createImageSFML(imageRaster);

    texRaster = sf::Texture(testImageRaster);
  }

  if (ImGui::Button("Sampler Bilineal", buttonSize2)) {

    imageOptionalRaster.CreateImage(imageRaster.m_width,
                                    imageRaster.m_height,
                                    imageRaster.m_bpp);

    for (int iY = 0; iY < image.m_height; iY++)
    {
      for (int iX = 0; iX < imageRaster.m_width; iX++)
      {
        float siU = (1.0f / imageRaster.m_width) * iX;
        float siV = (1.0f / imageRaster.m_height) * iY;
        Color color(imageRaster.SampleBilineal(siU, siV));
        imageOptionalRaster.SetPixel(iX, iY, color);
      }
    }

    setTextureInTex(imageOptionalRaster, testImageRaster, texRaster);

    imageRaster = imageOptionalRaster;
  }

  ImGui::SameLine();

  if (ImGui::Button("Gray Scale", buttonSize2)) {

    imageOptionalRaster = imageRaster.ProcessImage(ImageProceUI_APP::greyScale);

    setTextureInTex(imageOptionalRaster, testImageRaster, texRaster);

    testImageRaster = createImageSFML(imageRaster);

    imageRaster = imageOptionalRaster;
  }

  ImGui::Separator();
  ImGui::Text("EDGE DETECTOR");
  ImGui::Separator();

  const char* items[] = { "Outline", "Left Sobel", "Top Sobel", "Sum Sobel", "Left Prewwit",
                          "Top Prewwit" };

  static int32 current_item = 0;
  ImGui::Text("Types Kernels");
  ImGui::SetNextItemWidth(300);
  if (ImGui::BeginCombo("Types Kernels", items[current_item])) {
    for (int32 n = 0; n < IM_ARRAYSIZE(items); ++n) {
      const bool is_selected = (current_item == n);

      if (ImGui::Selectable(items[n], is_selected)) {
        current_item = n;
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  if (ImGui::Button("Apply Kernerl", buttonSize)) {
    imageOptionalRaster = ImageProceUI_APP::resolveImageKebler(imageRaster,
                                                               static_cast<KEBLERS::E>(current_item + 1));

    //imageExport = image.ProcessImage(ImageProceUI_APP::greyScale);

    setTextureInTex(imageOptionalRaster, testImageRaster, texRaster);

    testImageRaster = createImageSFML(imageRaster);

    imageRaster = imageOptionalRaster;
  }

  ImGui::End();
}

void
setTextureInTex(Image& image, sf::Image& imageSFML, sf::Texture& texImage) {
  imageSFML = createImageSFML(image);

  texImage = sf::Texture(imageSFML);
}

std::string TCHARToString(const TCHAR* txt)
{
#ifdef UNICODE
  int size_needed = WideCharToMultiByte(CP_UTF8, 0, txt, -1, NULL, 0, NULL, NULL);
  std::string str(size_needed, 0);
  WideCharToMultiByte(CP_UTF8, 0, txt, -1, &str[0], size_needed, NULL, NULL);
  if (!str.empty() && str.back() == '\0')
    str.pop_back(); // quitar null extra
  return str;
#else
  return std::string(txt);
#endif
}

void
exportImage()
{
  OPENFILENAME ofn;        // estructura del diálogo
    TCHAR szFile[MAX_PATH] = { 0 };  // buffer para ruta final

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = window.getNativeHandle();                 // ventana padre
    ofn.lpstrFile = szFile;               // buffer donde se guarda la ruta
    ofn.nMaxFile = sizeof(szFile);

    // Filtros (primero texto visible, luego patrón con terminación doble \0)
    ofn.lpstrFilter =
        TEXT("Bitmap Files (*.bmp)\0*.bmp\0")
        TEXT("PNG Files (*.png)\0*.png\0")
        TEXT("All Files (*.*)\0*.*\0");

    ofn.nFilterIndex = 1;                 // filtro por defecto
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    // Extensión por defecto si el usuario no pone una
    ofn.lpstrDefExt = TEXT("bmp");

    if (GetSaveFileName(&ofn))
    {
      auto codec = g_CodecMan.GetCodecByFileExt(".bmp");

      codec->Encode(imageExport, szFile);
  }
}