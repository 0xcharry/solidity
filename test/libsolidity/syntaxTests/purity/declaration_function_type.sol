==== Source: a ====
function fooA () {}

library ALib {
    function fooALib () public {}
}

==== Source: b ====
import "a" as Mod;
function foo () {}

library Lib {
    function fooLib () public {}
}

contract C {
    function fooContract () public {}

    function f() public pure {
        Mod.fooA;
        fooContract; // FIXME: This should generate warning too.
        C.fooContract;
        // TODO: Decide on these 3 cases. This function compiles so these expression should be pure, but they are not
        // TODO: marked as pure in the TypeChecker implementation, so they don't generate warnings.
        Lib.fooLib;
        Mod.ALib.fooALib;
        // this.fooContract; // Function declared as pure, but this expression (potentially) reads from the environment or state and thus requires "view".
    }
}
// ----
// Warning 6133: (b:180-188): Statement has no effect.
// Warning 6133: (b:263-276): Statement has no effect.
