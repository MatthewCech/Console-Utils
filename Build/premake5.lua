-- Premake5 Wiki: https://github.com/premake/premake-core/wiki
-- Based on Floofs_Inc Premake GLFW demo courtesy of JohannesMP


-- Variable definition
-- Brief reminder: This is actual lua, functions and whatnot are totally allowed.
local ROOT = "../"   -- Path to project root

---------------------------------
-- [ WORKSPACE CONFIGURATION   --
---------------------------------
workspace "Console_Utilities"                  -- Solution Name
  configurations { "Debug", "Release", "Lib"}  -- Optimization/General config mode in VS
  platforms { "x64", "x86"}                    -- Dropdown platforms section in VS
  
  local proj_action = "UNDEFINED"

  -- _ACTION is the argument passed into premake5 when you run it.
  if _ACTION ~= nill then
    proj_action = _ACTION
  end

  location (ROOT .. "project_" .. proj_action)

  -------------------------------
  -- [ COMPILER/LINKER CONFIG] --
  -------------------------------
  
  flags "FatalWarnings"  -- Warnings to 11! (all warnings on)
  warnings "Extra"       -- Extra warnings, such as unused variables

  filter { "platforms:*86" } architecture "x86"
  filter { "platforms:*64" } architecture "x64"

  filter "configurations:Debug"    defines { "DEBUG" }  symbols  "On"
  filter "configurations:Release"  defines { "NDEBUG" } optimize "On"
  filter "configurations:Lib"      defines { "NDEBUG", "STATIC_LIB" } optimize "On"

  filter { "action:gmake" }
    toolset "clang"
    buildoptions { "-std=c++14" }
    
  filter {} -- clear filter
  language "C++"


  -------------------------------
  -- [ PROJECT CONFIGURATION ] --
  ------------------------------- 
  project "ASCII_Draw"        -- Project name
    filter "configurations:not Lib"
      targetname "ascii_draw" -- Executable name
      kind "ConsoleApp"      -- Style of app in project- WindowedApp, ConsoleApp, etc.
    
    filter "configurations:Lib"
      targetname "canvas"
      kind "StaticLib"

    filter {} -- clear filter


  ------------------------------
  -- [ BUILD CONFIGURATIONS ] --
  ------------------------------

    -- Set the rpath on the executable, to allow for relative path for dynamic lib
    filter { "system:macosx", "action:gmake" }
      linkoptions { "-rpath @executable_path/lib" }

    filter { "system:linux", "action:gmake" }
      buildoptions { "-stdlib=libc++" }     -- linux needs more info
      linkoptions  { "-stdlib=libc++" }     
    
    -- when building any visual studio project
    filter { "system:windows", "action:vs*"}
      flags         { "MultiProcessorCompile", "NoMinimalRebuild" }
      linkoptions   { "/ignore:4099" }      -- Ignore library pdb warnings when running in debug

    filter {} -- clear filter   



    ----------------------------------
    -- [ FILE PATH CONFIGURATIONS ] --
    ----------------------------------
    local output_dir_root         = ROOT .. "bin_%{cfg.platform}_%{cfg.buildcfg}_" .. proj_action .. "/"
    filter {"configurations:not Lib"} targetdir(output_dir_root) -- Where all output files are stored
    filter {"configurations:Lib"}     targetdir(output_dir_root .. "/lib_%{cfg.platform}")
    filter {} -- clear filter
    
    local output_dir_lib          = output_dir_root .. "libs/" -- Mac Specific
    local output_dir_includes     = output_dir_root .. "includes/"

    local source_dir_root         = ROOT .. "Source/"
    local source_dir_engine       = source_dir_root .. "Code/"
    local source_dir_dependencies = source_dir_root .. "Dependencies/"

    local source_dir_includes     = source_dir_dependencies .. "**/Includes/"
    local source_dir_libs         = source_dir_dependencies .. "**/" .. "Libs_" .. os.get() .. "/"
    -- optional for libs that are 32 or 64 bit specific
    local source_dir_libs32       = source_dir_libs .. "lib_x32/"
    local source_dir_libs64       = source_dir_libs .. "lib_x64/"


    -- Files to be compiled (cpp) or added to project (visual studio)
    files
    {
      source_dir_engine .. "**.c",
      source_dir_engine .. "**.cpp",
      source_dir_engine .. "**.h",
      source_dir_engine .. "**.hpp",
      source_dir_engine .. "**.tpp",
    }

    filter { "files:**.tpp" }
      flags {"ExcludeFromBuild"}

    -- Ignore files for other operating systems (not necessary in this project)
    filter { "system:macosx" } removefiles { source_dir_engine .. "**_windows.*" }
    filter { "system:windows" } removefiles { source_dir_engine .. "**_macosx.*"  }
    
    filter { "configurations:Lib"} removefiles { source_dir_engine .. "main.cpp" }
    filter {} -- clear filter

    -- Handle 

    -- TODO: add 'vpaths' to organize filters in visual studio.

    vpaths {
      ["Headers"] = 
          { 
            source_dir_engine .. "**.h", 
            source_dir_engine .. "**.hpp"
          },
      ["Sources/*"] = 
        {
          source_dir_engine .. "**.c", 
          source_dir_engine .. "**.cpp", 
          source_dir_engine .. "**.tpp"
        },
      ["Docs"] = "**.txt"
    }

    -- Where compiler should look for library includes
    -- NOTE: for library headers always use  '#include <LIB/lib.hpp>' not quotes
    includedirs
    {
      source_dir_includes
    }
    

    -- Where linker should look for library files
    -- NOTE: currently each library must have "LIBS_<OS>" in its path.
    libdirs
    {
      source_dir_libs,                                           -- default: look for any libs here (does not recurse)
      source_dir_libs .. "lib_%{cfg.platform}/",                 -- libs with ONLY x32/x64 (no Release/Debug) versions
      source_dir_libs .. "%{cfg.buildcfg}/",                     -- libs with ONLY Release/Debug (no x32/x64) versions
      source_dir_libs .. "%{cfg.buildcfg}/lib_%{cfg.platform}/", -- libs with BOTH Release/Debug AND x32/x64 versions
      source_dir_libs .. "lib_%{cfg.platform}/%{cfg.buildcfg}/"  -- libs with BOTH x32/x64 AND Release/Debug versions (order reversed)
    }

    
    -- OS-specific Libraries - Dynamic libs will need to be copied to output

    --[[
    filter { "system:windows" }  -- Currently all static libs; No copying
      links
      {
        "glew32s",               -- static lib
        "glu32",                 -- Windows-specific for GLEW
        "glfw3",                 -- static lib
        "opengl32",
      }
    filter { "system:macosx" }   -- Currently all dylibs; Copy in postbuild
      links
      {
        "GLEW",
        "glfw3",
        "X11.6",                 -- Mac-specific for GLFW
        "pthread",               -- Mac-specific for GLFW
        "Xrandr.2",              -- Mac-specific for GLFW
        "Xi.6",                  -- Mac-specific for GLFW
        "Cocoa.framework",       -- Mac-specific for GLFW
        "IOKit.framework",       -- Mac-specific for GLFW
        "CoreVideo.framework",   -- Mac-specific for GLFW
        "OpenGL.framework",
      }
    filter {} -- clear filter
  --]]

-----------------------------------
-- POST-BUILD CONFIGURATIONS
-----------------------------------

-- temporary line for debugging
local copyline = "{COPY} " .. ROOT .. "Console-Utils.hpp " .. output_dir_includes .. "Console-Utils.hpp "

-- printed during `premake5 gmake`
-- printf("copyline is: \"%s\"", copyline)

-- run during `make config=lib_x64"
filter{"configurations:Lib, system:windows"}
  postbuildcommands 
  {
    "{RMDIR} " .. output_dir_includes,
    "{MKDIR} " .. output_dir_includes
	-- copyline
  }
  
filter{"configurations:Lib, system:not windows"}
  postbuildcommands
  {
    "{MKDIR} " .. output_dir_includes
    copyline
  }  

filter {} -- clear filter

