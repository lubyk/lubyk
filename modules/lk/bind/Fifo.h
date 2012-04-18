namespace lk {

/** Dummy definition to create the bindings to subclasses of the
 * lubyk::Fifo template.
 */
class FifoMethods {
public:
  int fd();
  LuaStackSize pop(lua_State *L);
};
}
