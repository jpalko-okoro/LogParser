// Standard Library Includes
#include <iostream>
#include <stdio.h>  // printf, fprintf
#include <stdlib.h> // abort

// DearImGui Includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

// GLFW Includes
#include <GLFW/glfw3.h>

// Vulcan Includes
#include <vulkan/vulkan.h>

int main(int, char **) {
  std::string testStr = "Test Start";
  std::cout << testStr;
  ImGui::Text("Hello, world %d", 123);
}
