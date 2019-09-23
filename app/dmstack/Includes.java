import es.dm.Std;

class Includes {
  static String output = "is.h";

  static void add (String dir) {
    for (String f : Std.dir(dir)) {
      String path = dir + "/" + f;
      if (Std.isDirectory(path)) {
        add(path);
      } else {
        if (path.endsWith(".h"))
          Std.append(output, Std.read(path) + "\n");
      }
    }
  }

  public static void main (String[] args) {
    if (!Std.fexists("dmstack.geany"))
      throw new IllegalStateException(
        "Current dir is not '/dm/dmC/app/dmstack'"
      );
    Std.write(output, "");
    add("include");
    add("lib");
  }
}
