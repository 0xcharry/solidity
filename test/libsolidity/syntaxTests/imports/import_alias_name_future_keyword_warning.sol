==== Source: a ====
contract A {}
==== Source: b ====
import {A as super} from "a";
contract C is super {}
// ----
// DeclarationError 3726: (b:13-18): The name "super" is reserved.
// Warning 2319: (b:13-18): This declaration shadows a builtin symbol.
