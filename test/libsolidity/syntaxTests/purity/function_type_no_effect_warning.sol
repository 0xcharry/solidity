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
    function fooContract () public pure {}

    function f() public pure {
        Mod.fooA;
        fooContract;
        C.fooContract;
        Lib.fooLib;
        Mod.ALib.fooALib;
    }
}
// ----
// Warning 6133: (b:197-202): Statement has no effect.
// Warning 6133: (b:212-224): Statement has no effect.
// Warning 6133: (b:234-250): Statement has no effect.
// Warning 6133: (b:260-274): Statement has no effect.
// Warning 6133: (b:284-301): Statement has no effect.
// Warning 6133: (b:311-333): Statement has no effect.
