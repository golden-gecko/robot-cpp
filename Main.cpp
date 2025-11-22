#include <iostream>
#include <map>

#include <boost/asio.hpp>
#include <boost/assign.hpp>

#include <OIS.h>

#include "Input.hpp"
#include "Window.hpp"

enum class myCommand
{
  DRIVE_FORWARD = 1,
  DRIVE_BACKWAD = 2,
  TURN_LEFT = 3,
  TURN_RIGHT = 4,
  TURN_SENSOR_LEFT = 5,
  TURN_SENSOR_RIGHT = 6,
  TURN_HEAD_LIGHTS_ON = 7,
  TURN_HEAD_LIGHTS_OFF = 8,
  TURN_STOP_LIGHTS_ON = 9,
  TURN_STOP_LIGHTS_OFF = 10,
  TURN_LEFT_SIGNAL_ON = 11,
  TURN_LEFT_SIGNAL_OFF = 12,
  TURN_RIGHT_SIGNAL_ON = 13,
  TURN_RIGHT_SIGNAL_OFF = 14,
  POWER_ON = 15,
  POWER_OFF = 16,
  STOP = 17
};

Input input;

boost::asio::io_service io;
boost::asio::serial_port serial(io, "COM20");

void send(myCommand command)
{
  int value = static_cast<int>(command);

  boost::asio::write(serial, boost::asio::buffer(&value, 1));

  std::cout << "Sending " << value << " command." << std::endl;
}

std::map<uint8_t, myCommand> bindings = boost::assign::map_list_of
  ('W', myCommand::DRIVE_FORWARD)
  ('S', myCommand::DRIVE_BACKWAD)
  ('A', myCommand::TURN_LEFT)
  ('D', myCommand::TURN_RIGHT)
  ('L', myCommand::TURN_HEAD_LIGHTS_ON)
  ('P', myCommand::POWER_ON)
  (VK_LEFT, myCommand::TURN_SENSOR_LEFT)
  (VK_RIGHT, myCommand::TURN_SENSOR_RIGHT);

std::map<uint8_t, myCommand> bindingsUp = boost::assign::map_list_of
  ('W', myCommand::STOP)
  ('S', myCommand::STOP)
  ('A', myCommand::TURN_LEFT_SIGNAL_OFF)
  ('D', myCommand::TURN_RIGHT_SIGNAL_OFF);

std::map<uint8_t, myCommand> bindingsControl = boost::assign::map_list_of
  ('L', myCommand::TURN_HEAD_LIGHTS_OFF)
  ('P', myCommand::POWER_OFF);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
    case WM_CLOSE:
      PostQuitMessage(0);
      break;

    case WM_KEYDOWN:
    {
      switch (wparam)
      {
        case VK_ESCAPE:
          PostQuitMessage(0);
          break;
      }

      // Find and send command.
      if (input.keys[wparam] == false)
      {
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
          auto command = bindingsControl.find(wparam);

          if (command != bindingsControl.end())
          {
            send(command->second);
          }
        }
        else
        {
          auto command = bindings.find(wparam);

          if (command != bindings.end())
          {
            send(command->second);
          }
        }
      }

      // Save pressed key.
      if (wparam < 256)
      {
        input.keys[wparam] = true;
      }

      std::cout << wparam << " pressed." << std::endl;
      break;
    }

    case WM_KEYUP:
    {
      // Find and send command.
      auto command = bindingsUp.find(wparam);

      if (command != bindingsUp.end())
      {
        send(command->second);
      }

      // Clear pressed key.
      if (wparam < 256)
      {
        input.keys[wparam] = false;
      }

      std::cout << wparam << " released." << std::endl;
      break;
    }

    case WM_SIZE:
      glViewport(0, 0, LOWORD(lparam), HIWORD(lparam));
      break;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

int main()
{
  Window window;
  window.init("Robot");

  DWORD last = GetTickCount();
  MSG msg;

  // Setup COM port.
  serial.set_option(boost::asio::serial_port_base::baud_rate(9600));

  // Enter main looop.
  do
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      DWORD current = GetTickCount();
      float delta = (current - last) / 1000.0f;
      last = current;

      SwapBuffers(window.hdc);
    }
  }
  while (msg.message != WM_QUIT);

  return 0;
}
