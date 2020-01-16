from render import render, Model

render(
    Model("axe.obj", texture_filename="axe.tga"),
    height=4000,
    width=4000,
    filename="result.png",
)
