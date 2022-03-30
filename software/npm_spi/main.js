import { LannooLeaf } from "./LannooLeaf.js";
import { Color } from "./color.js";

const lannoLeaf = new LannooLeaf();

async function main() {

    // lannoLeaf.set_unit_all(0xff, 0x00, new Color(50, 0, 0, 50));

    lannoLeaf.set_all_all(new Color(50, 0, 0, 50));
    await new Promise(r => setTimeout(r, 1000));

    lannoLeaf.set_all_all(new Color(0, 50, 0, 50));
    await new Promise(r => setTimeout(r, 1000));

    lannoLeaf.set_all_all(new Color(0, 0, 50, 50));
    await new Promise(r => setTimeout(r, 1000));
};

main();
