==== Source: a ====
uint constant CONST = 10;
==== Source: b ====
import "a" as super;
contract C {
    uint x = super.CONST;
}
// ----
// DeclarationError 3726: (b:0-20): The name "super" is reserved.
// Warning 6335: (b:0-20): "super" will be promoted to keyword in the next breaking version and will not be allowed as an identifier anymore.
// Warning 2319: (b:0-20): This declaration shadows a builtin symbol.
