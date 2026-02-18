# Dawn
A small C++ game and custom engine built with OpenGL, focused on simplicity and completion.

## TODO

- Write minimal GLSL shader (MVP + basic diffuse)
- Refine PhongMaterial to match shader contract
- Implement RawMaterial → PhongMaterial conversion
- Implement MeshRenderer::AttachModelToActor
- Implement Camera component (view + projection)
- Wire Renderer draw loop

# Model + Material + Rendering TODO

## Architecture Rules
- Separate CPU assets (Raw*) from GPU resources
- Shared resources live for entire app (for this project)
- Material is a value object owned by MeshRenderer
- No ref counting, no unloading system

---

## 1. CPU Asset Layer (AssetManager)

- RawModel [done]
  - Store path
  - vector<RawMesh>
  - vector<RawMaterial>

- RawMesh [done]
  - vertices (CPU)
  - indices (CPU)
  - materialIndex

- RawMaterial [done]
  - texture paths / references
  - scalar parameters

- CPU caches:
  - map<string, RawModel>
  - map<string, RawTexture>

---

## 2. GPU Shared Resources (AssetManager)

- Mesh (VAO, VBO, EBO)
  - cache: map<RawMesh*, Mesh>

- Texture (GL handle)
  - cache: map<RawTexture*, Texture>

- Shader
  - cache: map<string, Shader>

---

## 3. Runtime Layer

- MeshRenderer
  - Mesh* (non-owning)
  - Material (by value)

- Material
  - Shader* (non-owning)
  - Texture* (non-owning)
  - uniform values
  - copyable
  - no manual delete

---

## 4. AttachModel Flow

- Get RawModel from CPU cache (load if missing)
- For each RawMesh:
  - Get/Create GPU Mesh
  - Create default Material from RawMaterial
    - Get/Create GPU Textures
    - Assign default Shader (Phong)
  - Create MeshRenderer
  - Attach to Actor

---

## Sharing Rules

- 1 RawModel per file
- 1 GPU Mesh per RawMesh
- 1 GPU Texture per RawTexture
- 1 Material per MeshRenderer

---

## Implementation Order

1. RawModel / RawMesh / RawMaterial structs
2. CPU caching in AssetManager
3. GPU Mesh creation + cache
4. GPU Texture creation + cache
5. Material class
6. MeshRenderer
7. AttachModel()


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