#include "docs_builder.h"

void generate_index_html(const ProjectConfig* config)
{
	char path[512];
	snprintf(path, sizeof(path), "docs\\index.html");

	FILE* f = fopen(path, "w");
	if (!f)
	{
		logError("Failed to create index.html", path);
		return;
	}

	fprintf(f,
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"	<head>\n"
		"		<meta charset=\"UTF-8\" />\n"
		"		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n"
		"		<title>%s</title>\n"
		"		<link rel=\"stylesheet\" href=\"style.css\" />\n"
		"		<link href=\"https://fonts.googleapis.com/css2?family=Inter:wght@400;700&display=swap\" rel=\"stylesheet\" />\n"
		"	</head>\n"
		"	<body>\n"
		"		<div class=\"container\">\n"
		"			<aside class=\"sidebar\">\n"
		"				<h2>📘 %s</h2>\n"
		"				<nav>\n"
		"					<ul id=\"sidebar-menu\"></ul>\n"
		"				</nav>\n"
		"			</aside>\n"
		"			<main class=\"content\">\n"
		"				<section id=\"intro\">\n"
		"					<hr />\n"
		"					<h1>Introduction</h1>\n"
		"					<p>%s</p>\n"
		"					<hr />\n"
		"				</section>\n"
		"				<section id=\"license\">\n"
		"					<h2>License</h2>\n"
		"					<p>\n"
		"						This project is licensed under the\n"
		"						<a href=\"%s\" target=\"_blank\">%s</a>.\n"
		"					</p>\n"
		"				</section>\n"
		"			</main>\n"
		"		</div>\n"
		"		<script src=\"root/manifest.js\"></script>\n"
		"		<script src=\"main.js\"></script>\n"
		"	</body>\n"
		"</html>\n",
		config->title,
		config->title,
		config->description,
		config->license_url,
		config->license_name
	);

	fclose(f);
	logSuccess("index.html generated", path);
}

void generate_main_js(void)
{
	FILE* f = fopen("docs\\main.js", "w");
	if (!f)
	{
		logError("Failed to create main.js", "docs\\main.js");
		return;
	}

	fprintf(f,
"function buildMenu(obj, parentPath = '', isSubmenu = true, depth = 0)\n"
"{\n"
"	const ul = document.createElement('ul');\n"
"\n"
"	if (isSubmenu)\n"
"	{\n"
"		ul.classList.add('submenu');\n"
"		ul.style.setProperty('--depth', depth);\n"
"	}\n"
"\n"
"	for (const [key, val] of Object.entries(obj))\n"
"	{\n"
"		const li = document.createElement('li');\n"
"\n"
"		if (val && typeof val === 'object')\n"
"		{\n"
"			const btn = document.createElement('button');\n"
"			btn.className = 'dropdown-toggle';\n"
"			btn.textContent = `${key} ▾`;\n"
"			btn.style.paddingLeft = `${depth * 1.25}em`;\n"
"			li.appendChild(btn);\n"
"			li.appendChild(buildMenu(val, parentPath + key + '/', true, depth + 1));\n"
"		}\n"
"		else\n"
"		{\n"
"			const a = document.createElement('a');\n"
"\n"
"			if (key.endsWith('.html'))\n"
"			{\n"
"				a.href = 'root/' + parentPath + key;\n"
"				a.textContent = key;\n"
"				a.style.paddingLeft = `${depth * 1.25}em`;\n"
"			}\n"
"			else\n"
"			{\n"
"				a.href = `#${(parentPath + key).replace(/[./]/g, '_')}`;\n"
"				a.textContent = key;\n"
"				a.style.paddingLeft = `${depth * 1.25}em`;\n"
"			}\n"
"\n"
"			li.appendChild(a);\n"
"		}\n"
"\n"
"		ul.appendChild(li);\n"
"	}\n"
"\n"
"	return ul;\n"
"}\n"
"\n"
"function buildSidebarMenu(manifest)\n"
"{\n"
"	if (!manifest)\n"
"	{\n"
"		document.querySelector('.content').innerHTML =\n"
"			`<h1>Erreur</h1><p>Manifest introuvable ou invalide.</p>`;\n"
"		return;\n"
"	}\n"
"\n"
"	const menuRoot = document.getElementById('sidebar-menu');\n"
"	menuRoot.innerHTML = `<li><a href=\"#intro\">Introduction</a></li>`;\n"
"\n"
"	menuRoot.appendChild(buildMenu(manifest, '', false, 0));\n"
"	menuRoot.innerHTML += `<li><a href=\"#license\">License</a></li>`;\n"
"\n"
"	document.querySelectorAll('.dropdown-toggle').forEach(btn =>\n"
"	{\n"
"		btn.addEventListener('click', e =>\n"
"		{\n"
"			e.preventDefault();\n"
"			const submenu = btn.nextElementSibling;\n"
"			submenu.classList.toggle('visible');\n"
"			btn.classList.toggle('active');\n"
"		});\n"
"	});\n"
"\n"
"	document.querySelectorAll('.copy-btn').forEach(btn =>\n"
"	{\n"
"		btn.addEventListener('click', () =>\n"
"		{\n"
"			const code = btn.nextElementSibling.innerText;\n"
"			navigator.clipboard.writeText(code).then(() =>\n"
"			{\n"
"				btn.classList.add('copied');\n"
"				btn.innerText = '✅';\n"
"				setTimeout(() =>\n"
"				{\n"
"					btn.classList.remove('copied');\n"
"					btn.innerText = '📋';\n"
"				}, 1500);\n"
"			});\n"
"		});\n"
"	});\n"
"\n"
"	document.querySelectorAll('.sidebar a').forEach(link =>\n"
"	{\n"
"		link.addEventListener('click', () =>\n"
"		{\n"
"			document.querySelectorAll('.sidebar a').forEach(l => l.classList.remove('active'));\n"
"			link.classList.add('active');\n"
"		});\n"
"	});\n"
"}\n"
"\n"
"document.addEventListener('DOMContentLoaded', () => { buildSidebarMenu(manifest) });\n"
);

	fclose(f);
	logSuccess("main.js generated", "docs\\main.js");
}