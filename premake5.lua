-- DAWN

workspace "Dawn"
    configurations {"Debug", "Release"}
    architecture "x86_64"
    
    startproject "Application"

    include "Engine"
    include "Application"

    -- Engine for now, will be Application later