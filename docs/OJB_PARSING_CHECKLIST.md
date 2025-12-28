### Part 1: The Geometry (The "Ingredients")

Before you connect anything, you have raw lists of data.

**1. Geometric Vertices (`v`)**

* [X] **Understand what it is:** A point in 3D space.
* [ ] **Format check:** Usually `v x y z`. Sometimes `v x y z w` (where `w` is an optional weight, usually defaults to 1.0).
* [ ] **Decision - Storage:** You need a container that can grow dynamically, as you don't know how many vertices are in the file until you finish reading it.
* [ ] **Decision - Component Types:** Will you store them as `double` for precision or `float` for memory/GPU usage?

**2. Texture Coordinates (`vt`)**

* [ ] **Understand what it is:** A 2D point mapping the vertex to an image (UV mapping).
* [ ] **Format check:** Usually `vt u v`. Sometimes `vt u v w` (for 3D textures).
* [ ] **Decision - Optionality:** Not every model has these. Your parser must handle the case where these lists are empty.
* [ ] **Decision - Range:** UVs are usually 0.0 to 1.0, but can go outside that range (tiling). Do you need to clamp them? (Usually no).

**3. Vertex Normals (`vn`)**

* [ ] **Understand what it is:** A directional vector indicating where the surface points (used for lighting).
* [ ] **Format check:** `vn x y z`.
* [ ] **Decision - Normalization:** The file *should* have normalized vectors (length = 1), but often doesn't. Will you trust the file, or re-normalize them upon load?

---

### Part 2: The Topology (The "Recipe")

This is the most complex concept. A "Face" tells the renderer how to connect the ingredients from Part 1.

**4. Face Definitions (`f`)**

* [ ] **Understand what it is:** A list of indices pointing back to the data in Part 1.
* [ ] **The 4 Formats:** You must decide which of these variations you will support (usually all of them):
1. `f v v v` (Vertex only)
2. `f v/vt v/vt v/vt` (Vertex + Texture)
3. `f v//vn v//vn v//vn` (Vertex + Normal, note the double slash)
4. `f v/vt/vn v/vt/vn v/vt/vn` (All three)


* [ ] **Decision - Triangulation:** An OBJ face can contain 3 points (triangle), 4 points (quad), or more (N-gon).
* *Option A:* Store exactly what the file says (harder to render later).
* *Option B:* "Triangulate" on the fly. If you read a face with 4 points, split it into 2 triangles internally.



**5. Indexing Logic**

* [ ] **The "1-Based" Rule:** OBJ indices start at 1. C++ arrays start at 0. You need a conversion step.
* [ ] **The "Negative Index" Rule:** An index of `-1` means "the most recently added vertex". `-2` means "the second most recent".
* *Decision:* Will you support relative (negative) indexing? (It is rare in static models but valid in the spec).



---

### Part 3: Grouping & State (The "Context")

OBJ is a state-machine format.

**6. Object Names (`o`) and Groups (`g`)**

* [ ] **Understand what they are:** Logical separators. `o` usually defines a distinct mesh (like "Head"), `g` defines a sub-group (like "Jaw").
* [ ] **Decision - Storage Hierarchy:** Do you want to load the whole file as one big blob of triangles? Or do you want a list of "Mesh Objects," each containing its own vertices and faces?

**7. Materials (`mtllib` & `usemtl`)**

* [ ] **Understand what they are:** `mtllib` names an external file (e.g., `model.mtl`). `usemtl` tells the parser "All faces after this line use Material X".
* [ ] **Decision:** For a simple parser, you might ignore this. If you support it, you need to store a "Material ID" alongside every Face.

---

### Part 4: Data Validation (The "Sanity Check")

How to store the data once you understand it.

**8. The "Soup" vs. "Indexed" decision**

* **The Problem:** In OBJ, a vertex position can be shared by 5 faces, but have a different normal for each face (sharp edges).
* **Decision:**
* *Approach A (Raw OBJ):* Store exactly what OBJ gives you: 3 separate lists (Pos, UV, Norm) and a Face structure that holds 3 separate indices for every corner.
* *Approach B (Game Ready):* GPU vertex buffers need a "Unique Vertex" to be a specific combination of Pos+UV+Norm. If a position has two different normals, it needs to be duplicated in the vertex buffer. *Will you do this conversion during parsing or later?*


Parsing will be done primarly via string_view and from_chars due to performance. std::views::split to parse line by line. And mmap for zero-coping (though windows has different API CreateFileMapping)
