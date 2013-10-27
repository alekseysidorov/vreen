import qbs.base 1.0
import qbs.FileInfo
import qbs.TextFile
import qbs.File

Module {
    property string binDestination: project.bin_path
    property string libDestination: project.lib_path
    property string qmlDestination: project.qml_path

    property string versionMajor:  1
    property string versionMinor: 8
    property string versionRelease: 0

    Depends { name: "cpp" }
    Depends { name: "Qt.core"}

    setupRunEnvironment: {
        putenv('QML2_IMPORT_PATH', product.buildDirectory + "/" + qmlDestination);
    }

    cpp.cxxFlags: {
        var flags = base.concat("-std=c++11");
        if (qbs.toolchain.contains("clang"))
            flags = flags.concat("-stdlib=libc++");
        return flags;
    }

    Rule {
        inputs: [ "devheader" ]
        Artifact {
            fileTags: [ "hpp",  "installed_content" ]
            fileName: "GeneratedFiles/" + input.moduleProperty("qbs", "installDir") + "/" + input.fileName
        }

        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.sourceCode = function() {
                var inputFile = new TextFile(input.fileName, TextFile.ReadOnly);
                var file = new TextFile(output.fileName, TextFile.WriteOnly);
                file.truncate();
                file.write("#include \"" + input.fileName + "\"\n");
                file.close();
            }
            cmd.description = "generating " + FileInfo.fileName(output.fileName);
            cmd.highlight = "filegen";
            return cmd;
        }
    }

    Rule {
        inputs: [ "qml" ]
        Artifact {
            fileTags: [ "installed_content" ]
            fileName: input.moduleProperty("qbs", "installDir") + "/" + input.fileName
        }

        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.sourceCode = function() {
                File.remove(output.fileName);
                if (!File.copy(input.fileName, output.fileName))
                    throw "Cannot install '" + input.fileName + "' as '" + output.fileName + "'";
            }
            cmd.description = "copying " + FileInfo.fileName(output.fileName);
            cmd.highlight = "linker";
            return cmd;
        }
    }
}
