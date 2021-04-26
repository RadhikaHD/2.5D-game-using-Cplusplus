# A Playable 2.5d game in C++

This was my final submission for the Software Design course at Clemson University.

![Demo!](images/output.gif)

Game Strategy:

1) Blossom's health decreases over time.
2) Blossom's health is regained when after attaining Chemical X, which tries to evade her when approached.
3) Blossom's health diminishes when she collides with Mojo Jojo
3) Blossom shoots and explodes Mojo Jojo.

Win strategy: Blossom shoots Mojo Jojo before her health diminishes completely.
Lose strategy: Blossom's health diminishes completely before Mojo Jojo is shot.

How I made my sprites:

* Background 1 and 2 made from scratch by using Inkscape image editor.
* Foreground sprite for snow made from scratch using gimp image editor.
* Foreground sprite partially made by using Inkscape image editor.
* Note: I got the basic Foreground images from the following locations on the internet and then edited it in Inkscape to create a sprite sheet.
* Source for ChemicalX sprite: http://www.clker.com/clipart-256444.html
* Source for Mojo sprite: http://kingdom-hearts-the-last-stand.wikia.com/wiki/File:Mojo_Jojo.gif
* Source for Blossom sprite: http://powerpuffgirls.wikia.com/wiki/Blossom/Gallery

Music:

* Theme song source: https://youtu.be/mBfG-ZsHI0s
* Other sounds: Provided in class

How to compile code:
make <press enter key>

How to run code:
./run <press enter key>

How to generate frames for a movie:
Press F4 on the keyboard.
