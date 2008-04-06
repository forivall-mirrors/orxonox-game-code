/*
** Lua binding: script
** Generated automatically by tolua++-1.0.92 on Sat Apr  5 20:29:18 2008.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_script_open (lua_State* tolua_S);

#include "Script.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_orxonox__Script (lua_State* tolua_S)
{
 orxonox::Script* self = (orxonox::Script*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"orxonox::Script");
}

/* method: new of class  orxonox::Script */
#ifndef TOLUA_DISABLE_tolua_script_orxonox_Script_new00
static int tolua_script_orxonox_Script_new00(lua_State* tolua_S)
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
   orxonox::Script* tolua_ret = (orxonox::Script*)  Mtolua_new((orxonox::Script)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"orxonox::Script");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  orxonox::Script */
#ifndef TOLUA_DISABLE_tolua_script_orxonox_Script_new00_local
static int tolua_script_orxonox_Script_new00_local(lua_State* tolua_S)
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
   orxonox::Script* tolua_ret = (orxonox::Script*)  Mtolua_new((orxonox::Script)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"orxonox::Script");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: luaPrint of class  orxonox::Script */
#ifndef TOLUA_DISABLE_tolua_script_orxonox_Script_luaPrint00
static int tolua_script_orxonox_Script_luaPrint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"orxonox::Script",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  orxonox::Script* self = (orxonox::Script*)  tolua_tousertype(tolua_S,1,0);
  char* str = ((char*)  tolua_tostring(tolua_S,2,0));
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
TOLUA_API int tolua_script_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"orxonox",0);
  tolua_beginmodule(tolua_S,"orxonox");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Script","orxonox::Script","",tolua_collect_orxonox__Script);
   #else
   tolua_cclass(tolua_S,"Script","orxonox::Script","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Script");
    tolua_function(tolua_S,"new",tolua_script_orxonox_Script_new00);
    tolua_function(tolua_S,"new_local",tolua_script_orxonox_Script_new00_local);
    tolua_function(tolua_S,".call",tolua_script_orxonox_Script_new00_local);
    tolua_function(tolua_S,"luaPrint",tolua_script_orxonox_Script_luaPrint00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_script (lua_State* tolua_S) {
 return tolua_script_open(tolua_S);
};
#endif

