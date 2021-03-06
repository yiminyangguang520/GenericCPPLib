﻿#ifndef FuncUtilsH
#define FuncUtilsH

//// [!] Version 1.01 [!]

#include <utility>     // for 'std::move', 'std::forward'
#include <type_traits> // for 'std::is_same'

//// A functor (Function Object) CAN also be possibly used as a 'ProcName' OR 'FuncName'
#define EXEC_MEMBER_PROC_IF_PRESENT(ProcName) namespace ProcName {\
  void ExecIfPresent(...) throw() {}\
  \
  template <class C, typename... TArgs>\
  auto ExecIfPresent(C& obj, TArgs&&... args)\
    /*Remove prototype from the overload resolution if such a callable (func./functor) is NOT exist*/\
    -> decltype(obj.ProcName(std::forward<TArgs>(args)...))\
  {\
    static_assert(std::is_same<void,\
                               typename decltype(obj.ProcName(std::forward<TArgs>(args)...))>::value,\
                  "'EXEC_MEMBER_PROC_IF_PRESENT' SHOULD be used with the procedures,"\
                  " which returns nothing (void)");\
    obj.ProcName(std::forward<TArgs>(args)...); /*MS VS is OK with the 'return void'*/\
  }\
};

// If NOT exists - returns the 'DefaultValue',
//  which SHOULD be the same type as a decltype(*.FuncName())
// Works with static/const/virtual funcs
#define EXEC_MEMBER_FUNC_IF_PRESENT(FuncName, DefaultValue) namespace FuncName {\
  template <typename TReturnType = decltype(DefaultValue)>\
  auto ExecIfPresent(...) -> TReturnType {\
    return std::move(DefaultValue);\
  }\
  \
  template <class C, typename... TArgs>\
  auto ExecIfPresent(C& obj, TArgs&&... args)\
    /*Remove prototype from the overload resolution if such a callable (func./functor) is NOT exist*/\
    -> decltype(obj.FuncName(std::forward<TArgs>(args)...)) /*NOT 'const C& obj' NOR 'C::FuncName()'*/\
  {\
    return std::move(obj.FuncName(std::forward<TArgs>(args)...));\
  }\
};

#endif // FuncUtilsH