1. Install reason-sdl2

`esy add reason-sdl2`

2. Add it to dune

[link](https://dune.readthedocs.io/en/stable/concepts.html#foreign-sources-and-archives)

with dune 2.0

[link](https://github.com/ocaml/dune/blob/1.11.4/doc/foreign-code.rst)

[discord](https://cdn.discordapp.com/attachments/235200837608144898/660476240594993152/unknown.png)

with the old dune:

(library
(name sdl2)
(public_name sdl2)
(c_names sdl2)
(c_flags -I%{env:SDL2_INCLUDE_PATH=default}))

BUT for env to work dune-project needs to have

(lang dune 1.4)

OR

(lang dune 1.11)
(using fmt 1.2)

[link](https://github.com/revery-ui/esy-sdl2/blob/master/package.json#L16)
