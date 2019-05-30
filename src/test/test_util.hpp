#ifndef TEST_UTIL_HPP
#define TEST_UTIL_HPP
#include "../core/rendering/GLFWWindow.hpp"
template <typename F, typename... Args>
void executeTest(size_t runs, F&& func, Args&&... args) {
#ifdef MARKERS
  while (GLFWWindow::shouldClose()) {
#else
  for (size_t run = 0; run < runs; run++) {
#endif
    std::forward<F>(func)(std::forward<Args>(args)...);
#ifdef MARKERS
    GLFWWindow::clear();
    GLFWWindow::swapBuffers();
#endif
  }
}
#endif

