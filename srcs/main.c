#include "docs_builder.h"

//
// ┌──────┐
// │ MAIN │
// └──────┘
//
int main(void)
{
	logInfo("Starting root folder generation", NULL);
	CreateDirectoryA(OUTPUT_FOLDER, NULL);
	logSuccess("Creation root folder success in ", OUTPUT_FOLDER);

	logInfo("Starting docs generation", NULL);
	int result = scan_and_create_docs(INPUT_FOLDER, "");
	if (result)
		logSuccess("Filtered folder structure with docs placeholders generated in ", OUTPUT_FOLDER);
	else
		logError("No Lua files with doc comments found.", NULL);

	logInfo("Starting manifest generation", NULL);

	if (!isDirectory(ROOT_PATH))
		return (logError("ROOT_PATH not found", ROOT_PATH), 1);

	char sOutputPath[4096];
	snprintf(sOutputPath, sizeof(sOutputPath), "%s\\%s", ROOT_PATH, OUTPUT_FILE);

	FILE *fOut = fopen(sOutputPath, "w");
	if (!fOut)
		return (logError("Failed to create output file", sOutputPath), 1);

	logInfo("Writing to file", sOutputPath);

	fprintf(fOut, "const manifest = {\n");
	writeTree(fOut, ROOT_PATH, 1);
	fprintf(fOut, "\n};\n");
	fclose(fOut);

	return (logSuccess("Manifest generated", sOutputPath), EXIT_SUCCESS);
}
