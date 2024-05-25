## Guidelines

Some notes about the graphic language to adopt when using Mermaid diagrams:

- If something should be done (now or in the future), mark it with <span style="color:#ff0000">todo</span>.

- If something has been quirkly implemented, mark it with <span style="color:#fcba03">alpha</span>.

- If something has been refined, mark it with <span style="color:#fc03a9">beta</span>.

- If something has been tested and is ready for staging, mark it with <span style="color:#03fcd7">staging</span>.

- If something has been shipped, mark it with <span style="color:#00ff00">done</span>.

## Class Diagram

```mermaid
classDiagram
  class Result {
    Result : Err(data)
    Result : Ok(data)
  }
  <<Monad>> Result

  class State {
    update()
  }
  <<Namespace>> State

  class DirectSolver {
    fn run(self, A: Matrix, b: Vector) -> Result;
  }
  <<Trait>> DirectSolver
  DirectSolver --> Result : uses

  class BackwardSubstitutionSolver
  BackwardSubstitutionSolver --> DirectSolver : implements

  class ForwardSubstitutionSolver
  ForwardSubstitutionSolver --> DirectSolver : implements

  class IndirectSolver {
    fn run(self, A: Matrix, b: Vector, tol: Float, maxIter: Int) -> Result;
  }
  <<Trait>> IndirectSolver
  IndirectSolver --> Result : uses

  class IterativeEngine {
    fn post_initialize(state: State) -> None;
    fn pre_compute_y(state: State) -> None;
    fn compute_y(state: State) -> None;
    fn post_compute_x(state: State) -> None;
  }
  <<Trait>> IterativeEngine
  IterativeEngine --> State : uses

  class JacobiEngine
  JacobiEngine --> IterativeEngine : implements

  class RichardsonEngine
  RichardsonEngine --> IterativeEngine : implements

  class GaussSeidelEngine
  GaussSeidelEngine --> IterativeEngine : implements

  class GradientEngine
  GradientEngine --> IterativeEngine : implements

  class ConjugateGradientEngine
  ConjugateGradientEngine --> IterativeEngine : implements

  class IterativeSolver {
    engine: IterativeEngine
    fn run(self, A: Matrix, b: Vector, tol: Float, maxIter: Int) -> Result;
  }
  IterativeSolver --> IterativeEngine : uses
  IterativeSolver --> State : uses
  IterativeSolver --> IndirectSolver : implements
```
