import qbs.base 1.0
import qbs.FileInfo
import qbs.TextFile
import qbs.File

Module {
    property string binDestination: project.vreen_bin_path
    property string libDestination: project.vreen_lib_path
    property string qmlDestination: project.vreen_qml_path

    Depends { name: "cpp" }
    Depends { name: "Qt.core"}

    setupRunEnvironment: {
        putenv('QML2_IMPORT_PATH', product.buildDirectory + "/" + qmlDestination);
    }

    cpp.cxxLanguageVersion: "c++11"

    Rule {
        inputs: [ "devheader" ]
        Artifact {
            fileTags: [ "hpp",  "installed_content" ]
            filePath: "GeneratedFiles/" + input.moduleProperty("qbs", "installDir") + "/" + input.fileName
        }

        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.sourceCode = function() {
                var inputFile = new TextFile(input.filePath, TextFile.ReadOnly);
                var file = new TextFile(output.filePath, TextFile.WriteOnly);
                file.truncate();
                file.write("#include \"" + input.filePath + "\"\n");
                file.close();
            }
            cmd.description = "generating " + FileInfo.fileName(output.filePath);
            cmd.highlight = "filegen";
            return cmd;
        }
    }

    Rule {
        inputs: [ "qml" ]
        Artifact {
            fileTags: [ "installed_content" ]
            filePath: input.moduleProperty("qbs", "installDir") + "/" + input.fileName
        }

        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.sourceCode = function() {
                File.remove(output.filePath);
                if (!File.copy(input.filePath, output.filePath))
                    throw "Cannot install '" + input.filePath + "' as '" + output.filePath + "'";
            }
            cmd.description = "copying " + FileInfo.fileName(output.filePath);
            cmd.highlight = "linker";
            return cmd;
        }
    }
}
