# Dawn
A small C++ game and custom engine built with OpenGL, focused on simplicity and completion.

## TODO
- 13 Feb → Rendering basics (Shader, Mesh, Model)
- 16 Feb → MeshRenderer component → first 3D draw

### AudioSystem
- Bank
- SoundEvents

### Actor
- PlayerActor

### Component (Base class)

### Rendering
- Model/Mesh
- Shader
- MeshRenderer
- SpriteRenderer
- SkinnedMeshRenderer

### GUI
- IMGui wrapper

### Layers
- 3 types of layers:
    - [Done] layer : a normal layer
    - [Done] gameplay layer : owns scene
    - overlay layer : normal layer, which is appended after normal and gameplay layer

### Scene
- [Done] owns a list of actors
- can load/ save those actors via serialization to a file "scene.json"
- Scenes are managed like assets, with a name and extension like "scene1.scene"
- Gameplay layer can use LoadScene("scene.scene), to create a new scene file if it doesn't exist, or load an existing scene file