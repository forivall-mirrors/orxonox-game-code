/*
** Lua binding: orxonox
** Generated automatically by tolua++-1.0.92 on Tue Apr 22 10:52:02 2008.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_orxonox_open (lua_State* tolua_S);

#include "core/Script_clean.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"orxonox::Script");
}

/* method: getInstance of class  orxonox::Script */
#ifndef TOLUA_DISABLE_tolua_orxonox_orxonox_Script_getInstance00
static int tolua_orxonox_orxonox_Script_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"orxonox::Script",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   orxonox::Script* tolua_ret = (orxonox::Script*)  orxonox::Script::getInstance();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"orxonox::Script");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: luaPrint of class  orxonox::Script */
#ifndef TOLUA_DISABLE_tolua_orxonox_orxonox_Script_luaPrint00
static int tolua_orxonox_orxonox_Script_luaPrint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"orxonox::Script",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  orxonox::Script* self = (orxonox::Script*)  tolua_tousertype(tolua_S,1,0);
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'luaPrint'",NULL);
#endif
  {
   self->luaPrint(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'luaPrint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_orxonox_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"orxonox",0);
  tolua_beginmodule(tolua_S,"orxonox");
   tolua_cclass(tolua_S,"Script","orxonox::Script","",NULL);
   tolua_beginmodule(tolua_S,"Script");
    tolua_function(tolua_S,"getInstance",tolua_orxonox_orxonox_Script_getInstance00);
    tolua_function(tolua_S,"luaPrint",tolua_orxonox_orxonox_Script_luaPrint00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_orxonox (lua_State* tolua_S) {
 return tolua_orxonox_open(tolua_S);
};
#endif

