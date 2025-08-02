object Elaborate extends App {
  // Disable Vec flattening to emit unpacked arrays for BlackBox IO
  System.setProperty("chisel.enableMemVecFlatten", "false")
  
  // Extract --rom=filename from args
  val romFile = args.find(_.startsWith("--rom=")).map(_.drop(6)).getOrElse("rom.txt")

  val firtoolOptions = Array(
    "--lowering-options=" + List(
      "disallowLocalVariables",
      "disallowPackedArrays",
      "locationInfoStyle=wrapInAtSquareBracket"
    ).reduce(_ + "," + _)
  )

  // Pass romFile into your Top module
  circt.stage.ChiselStage.emitSystemVerilogFile(
    new npc.Top(romFile = romFile),
    args.filterNot(_.startsWith("--rom=")), // remove --rom from Chisel args
    firtoolOptions
  )
}
