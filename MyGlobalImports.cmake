#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_init_global_import_variables)

   if(MSVC)
      set (MyRisLibIncludePath "C:/MyTools/MyLib/include/RisLib" PARENT_SCOPE)
      set (MyRisLibImportPath  "C:/MyTools/MyLib/lib/RisLib.lib" PARENT_SCOPE)
      set (MyDspLibIncludePath "C:/MyTools/MyLib/include/DspLib" PARENT_SCOPE)
      set (MyDspLibImportPath  "C:/MyTools/MyLib/lib/DspLib.lib" PARENT_SCOPE)
   elseif(CMAKE_SYSTEM_VERSION EQUAL 101)
      set (MyRisLibIncludePath "C:/MyTools/MyLib/include/RisLib" PARENT_SCOPE)
      set (MyRisLibImportPath  "C:/MyTools/MyLib/lib/libRisLib.so" PARENT_SCOPE)
      set (MyDspLibIncludePath "C:/MyTools/MyLib/include/DspLib" PARENT_SCOPE)
      set (MyDspLibImportPath  "C:/MyTools/MyLib/lib/libDspLib.so" PARENT_SCOPE)
   else()
      set (MyRisLibIncludePath "/opt/prime/include/RisLib" PARENT_SCOPE)
      set (MyRisLibImportPath  "/opt/prime/lib/libRisLib.so" PARENT_SCOPE)
      set (MyDspLibIncludePath "/opt/prime/include/DspLib" PARENT_SCOPE)
      set (MyDspLibImportPath  "/opt/prime/lib/libDspLib.so" PARENT_SCOPE)
   endif()

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_RisLib _target)

   if (MSVC)
      set (MyRisLibImportPath  "C:/MyTools/MyLib/lib/RisLib.lib")
   elseif (MYMODE STREQUAL beagle)
      set (MyRisLibImportPath  "C:/MyTools/MyLib/lib/libRisLib.a")
   else()
      set (MyRisLibImportPath  "/opt/prime/lib/libRisLib.a")
   endif()

   if (MSVC)
      add_library(RisLib STATIC IMPORTED)
      set_target_properties(RisLib PROPERTIES IMPORTED_LOCATION ${MyRisLibImportPath})
      target_link_libraries(RisLib INTERFACE ws2_32)
      target_link_libraries(RisLib INTERFACE winmm)
      target_link_libraries(${_target} RisLib)
   else()
      add_library(RisLib STATIC IMPORTED)
      set_target_properties(RisLib PROPERTIES IMPORTED_LOCATION ${MyRisLibImportPath})
      target_link_libraries(${_target} RisLib)
      target_link_libraries(${_target} pthread)
      target_link_libraries(${_target} rt)
      target_link_libraries(${_target} readline)
   endif()

endfunction()

function(my_inc_import_RisLib _target)

   if (MSVC OR MYMODE STREQUAL beagle)
      set (MyRisLibIncludePath "C:/MyTools/MyLib/include/RisLib")
   else()
      set (MyRisLibIncludePath "/opt/prime/include/RisLib")
   endif()

   target_include_directories(${_target} PUBLIC ${MyRisLibIncludePath})

endfunction()


#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_DspLib _target)

   if (MSVC)
      set (MyDspLibImportPath  "C:/MyTools/MyLib/lib/DspLib.lib")
   elseif (MYMODE STREQUAL beagle)
      set (MyDspLibImportPath  "C:/MyTools/MyLib/lib/libDspLib.so")
   else()
      set (MyDspLibImportPath  "/opt/prime/lib/libDspLib.so")
   endif()

   add_library(DspLib SHARED IMPORTED)
   set_target_properties(DspLib PROPERTIES IMPORTED_LOCATION ${MyDspLibImportPath})

   target_link_libraries(${_target} DspLib)

endfunction()

function(my_inc_import_DspLib _target)

   if (MSVC OR MYMODE STREQUAL beagle)
      set (MyDspLibIncludePath "C:/MyTools/MyLib/include/DspLib")
   else()
      set (MyDspLibIncludePath "/opt/prime/include/DspLib")
   endif()

   target_include_directories(${_target} PUBLIC ${MyDspLibIncludePath})

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

