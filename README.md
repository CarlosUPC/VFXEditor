

# VFX Shader Editor

Node-based shader prototype tool for visual effects creation as my final bachelor's thesis in Game Development at CITM, UPC. The project development wrapped up with a framework focused on creation and prototyping of shaders with
a self-made node graph editor. The final release comes with 16 available nodes along the master one in order to recreate a bunch of shaders from simple to advanced functionalities. In Addition, this tool includes a ShaderLab code translator to provide support in export Unity shader file format from the tool.

<p align="center">
<img src="https://github.com/CarlosUPC/VFXEditor/blob/main/Docs/img1.PNG" >
</p>

## Demo video:
You can see a demo video of my project's application: [here](https://www.youtube.com/watch?v=mgFD06GKWrQ)

## Instructions:

* To create a shader go to: Panel Graph Editor -> Asset -> Create Shader

* Right click in canvas and select a node type (check out the section below to see the available nodes)

* Modify the shader as you wish

* Node Graph Editor:
    * Drag node
    * Hover node
    * Select node (doble-click)
    * Drag spline from a socket
    * Connect splines from output socket's node to input socket's node and vice-versa
    * Right click in the node to delete it
    * Right click in the spline to delete it
    * Right click + ALT in the socket to disconnect a link from 2 nodes

* Modify values in Inspector panel selecting a node
* Press on Compile Button to recompile shader and see new changes
* Unity Exporter
    * Press on Export -> Export to Unity Button to generate a unity .shader file format where you can find it in Shaders/ShaderLab folder path
    * Drag & drog .shader file in Unity
    * Create a material to set up the shader on it and enjoy :D


## Available nodes:

### Constant
* Float node
* Vector2 node
* Vector3 node
* Vector4 node
* Color node
* Time node

### Math
* Add node
* Subtract node
* Multiply node
* Divide node

### Texture
* Texture node
* Texture Sampler node

## Geometry
* UV node
* Tiling & Offset node
* Panner
* Parallax Oclusion

## Master
* Unlit



## Source Code

* GitHub repository: [https://github.com/CarlosUPC/VFXEditor](https://github.com/CarlosUPC/VFXEditor)
* Demo Release: [https://github.com/CarlosUPC/VFXEditor](https://github.com/CarlosUPC/VFXEditor/releases/tag/v1.0)

## Author

* Carlos Peña Hernando [CarlosUPC](https://github.com/CarlosUPC)
