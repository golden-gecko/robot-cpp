#pragma once

#include <Windows.h>

#include <gl/GL.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class Window
{
public:
  WNDCLASS wc;
  HWND hwnd;
  HDC hdc;
  HGLRC hglrc;

  void init(const char title[])
  {
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = title;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
      WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
      title,
      title,
      WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      0,
      0,
      1366,
      768,
      NULL,
      NULL,
      GetModuleHandle(NULL),
      NULL
    );

    PIXELFORMATDESCRIPTOR pfd =
    {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,                              // Version Number
      PFD_DRAW_TO_WINDOW |            // Format Must Support Window
      PFD_SUPPORT_OPENGL |            // Format Must Support OpenGL
      PFD_DOUBLEBUFFER,               // Must Support Double Buffering
      PFD_TYPE_RGBA,                  // Request An RGBA Format
      32,                             // Select Our Color Depth
      0, 0, 0, 0, 0, 0,               // Color Bits Ignored
      0,                              // No Alpha Buffer
      0,                              // Shift Bit Ignored
      0,                              // No Accumulation Buffer
      0, 0, 0, 0,                     // Accumulation Bits Ignored
      16,                             // 16Bit Z-Buffer (Depth Buffer)
      0,                              // No Stencil Buffer
      0,                              // No Auxiliary Buffer
      PFD_MAIN_PLANE,                 // Main Drawing Layer
      0,                              // Reserved
      0, 0, 0                         // Layer Masks Ignored
    };

    hdc = GetDC(hwnd);
    GLuint pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);
    hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);
    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    GLfloat clearColor[4] = { 0.5f, 0.75f, 1.0f, 1.0f };

    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

    // Culling.
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Depth.
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    // Color material.
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    // Lighting.
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

    GLfloat LightAmbient [] =	{ 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat LightDiffuse [] =	{ 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat LightPosition[] =	{ 500.00f, 500.00f, 500.0f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT , LightAmbient );
    glLightfv(GL_LIGHT1, GL_DIFFUSE , LightDiffuse );
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

    // Hints.
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Shading.
    glShadeModel(GL_SMOOTH);

    // Lines.
    glLineWidth(1.0f);

    // Fog.
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, clearColor);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glHint(GL_FOG_HINT, GL_NICEST);
    glFogf(GL_FOG_START, 600.0f);
    glFogf(GL_FOG_END, 1200.0f);
    glEnable(GL_FOG);

    // Textures.
    glEnable(GL_TEXTURE_2D);
  }

  void destroy(const char title[])
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
    UnregisterClass(title, GetModuleHandle(NULL));
  }
};
