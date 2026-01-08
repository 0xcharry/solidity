==== Source: a ====
event EvGlobalMod();

contract A {
    event EvContractMod();
}
library LibMod {
    event EvLibMod();
}
==== Source: b ====
import "a" as Mod;

event EvGlobal();

library Lib {
    event EvLib();
}

contract C {
    event EvContract();

    function f() public pure {
        // FIXME: They should all generate warnings "Statement has no effect.".
        Mod.EvGlobalMod;
        Mod.A.EvContractMod;
        Mod.LibMod.EvLibMod;
        C.EvContract;
        Lib.EvLib;
        EvGlobal;
        EvContract;
    }
}
// ----
