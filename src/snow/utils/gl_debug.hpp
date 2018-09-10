#ifndef GL_DEBUG
#define GL_DEBUG
namespace debug {
static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                       GLenum severity, GLsizei length,
                                       const GLchar* message,
                                       const void* userParam) {
  if (!(type == GL_DEBUG_TYPE_PUSH_GROUP || type == GL_DEBUG_TYPE_POP_GROUP))
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
            severity, message);
}

struct MarkerRAII {
  MarkerRAII(std::string name) {
    glPushDebugGroup(GL_DEBUG_SOURCE_THIRD_PARTY, 0, 0, name.c_str());
  }
  ~MarkerRAII() noexcept { glPopDebugGroup(); }
};
}  // namespace debug
#endif

