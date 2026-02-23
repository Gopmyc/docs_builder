#include "docs_builder.h"

//
// ┌──────┐
// │ MAIN │
// └──────┘
//

// TODO: Create a version control system, a search bar, and a client/server differentiation system
int main(void)
{
	/* ───────────────────────────── */
	/*        YAML CONFIG LOAD       */
	/* ───────────────────────────── */
	ProjectConfig config;

	if (!load_config_yaml("docs_config.yaml", &config))
		return (printf("Failed to load 'docs_config.yaml'"), 1);

	set_runtime_config(&config.runtime);
	logInfo("Starting root folder generation", NULL);
	CreateDirectoryA(config.runtime.output_folder, NULL);
	logSuccess("Creation root folder success in ", config.runtime.output_folder);

	logInfo("Starting docs generation", NULL);
	int result = scan_and_create_docs(config.runtime.input_folder, "", &config);
	if (result)
		logSuccess("Filtered folder structure with docs placeholders generated in ", config.runtime.output_folder);
	else
		logError("No Lua files with doc comments found.", NULL);

	logInfo("Starting manifest generation", NULL);

	if (!isDirectory(config.runtime.root_path))
		return (logError("ROOT_PATH not found", config.runtime.root_path), 1);

	char sOutputPath[4096];
	snprintf(sOutputPath, sizeof(sOutputPath), "%s\\%s", config.runtime.root_path, config.runtime.output_file);

	FILE *fOut = fopen(sOutputPath, "w");
	if (!fOut)
		return (logError("Failed to create output file", sOutputPath), 1);

	logInfo("Writing to file", sOutputPath);

	fprintf(fOut, "const manifest = {\n");
	writeTree(fOut, config.runtime.root_path, 1, &config);
	fprintf(fOut, "\n};\n");
	fclose(fOut);

	logSuccess("Manifest generated", sOutputPath);

	/* ───────────────────────────── */
	/*      INDEX GENERATION         */
	/* ───────────────────────────── */

	logInfo("Generating index.html", NULL);
	generate_index_html(&config);

	logInfo("Generating style.css", NULL);
	generate_style_css(&config);

	logInfo("Generating main.js", NULL);
	generate_main_js(&config);

	return (EXIT_SUCCESS);
}
