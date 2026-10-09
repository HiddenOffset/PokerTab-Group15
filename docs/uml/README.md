# UML

| File | What |
| --- | --- |
| `naming-contract.md` | Every class, method, and actor name the diagrams and code share; marks implemented vs planned |
| `src/*.puml` | PlantUML sources — edit these, not the PNGs |
| `merged/*.png` | Rendered diagrams for PR2 |
| `slices/` | Per-member A2 slices (kept for handoff) |

## Re-render after editing a source

Needs Java. Download `plantuml.jar` from https://plantuml.com/download, then from this folder:

```
java -jar plantuml.jar -tpng -o ../merged src/*.puml
```

VS Code: the "PlantUML" extension previews a `.puml` live (Alt+D) and exports with
"PlantUML: Export Current Diagram".

## Keeping diagrams and code in step

The sequence diagrams only use classes from the class diagram and methods from the
naming contract. When you add a method in code, add it to `naming-contract.md` and
`class-diagram.puml` in the same pull request. Methods marked «planned» are the
ones still to implement; their names are fixed so the sequence diagrams stay valid.
