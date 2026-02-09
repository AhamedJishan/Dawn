# Dawn
A small C++ game and custom engine built with OpenGL, focused on simplicity and completion.

## TODO

- 9 Feb → Layers
- 10 Feb → Actor + Component (barebones)
- 11 Feb → Scene (list of actors, update loop only)
- 12 Feb → Integrate Scene into Layers
- 13 Feb → Rendering basics (Shader, Mesh, Texture)
- 14 Feb → MeshRenderer component → first 3D draw

### AudioSystem
- Bank
- SoundEvents

### Actor
- PlayerActor

### Component (Base class)

### Rendering
- Texture
- Model/Mesh
- Shader
- MeshRenderer
- SpriteRenderer
- SkinnedMeshRenderer

### GUI

### Layers
- 3 types of layers:
    - layer : a normal layer
    - gameplay layer : owns scene
    - overlay layer : normal layer, which is appended after normal and gameplay layer

### Scene
- owns a list of actors
- can load/ save those actors via serialization to a file "scene.json"
- Scenes are managed like assets, with a name and extension like "scene1.scene"
- Gameplay layer can use LoadScene("scene.scene), to create a new scene file if it doesn't exist, or load an existing scene file