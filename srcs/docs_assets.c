#include "docs_builder.h"

//
// ┌───────────────────────┐
// │ DOCS ASSETS GENERATOR │
// └───────────────────────┘
//

void generate_index_html(const ProjectConfig* config)
{
	char path[MAX_PATH];
	snprintf(path, sizeof(path), "%s\\index.html", config->runtime.output_folder);

	create_directory_recursive(config->runtime.output_folder);

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
		"				<div class=\"sidebar-search\"> <input type=\"text\" id=\"sidebarSearch\" placeholder=\"Search...\" /> </div>\n"
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
		"		<script src=\"%s\"></script>\n"
		"		<script src=\"%s\"></script>\n"
		"	</body>\n"
		"</html>\n",
		config->title,
		config->title,
		config->description,
		config->license_url,
		config->license_name,
		config->runtime.manifest_path,
		config->runtime.main_js_path
	);

	fclose(f);
	logSuccess("index.html generated", path);
}

void generate_main_js(const ProjectConfig* config)
{
	char path[MAX_PATH];
	snprintf(path, sizeof(path), "%s\\main.js", config->runtime.output_folder);

	create_directory_recursive(config->runtime.output_folder);

	FILE* f = fopen(path, "w");
	if (!f)
	{
		logError("Failed to create main.js", path);
		return;
	}

	fprintf(f,
"const defaultTheme={bgMain:getComputedStyle(document.documentElement).getPropertyValue('--bg-main').trim(),bgGradientStart:getComputedStyle(document.documentElement).getPropertyValue('--bg-gradient-start').trim(),bgGradientEnd:getComputedStyle(document.documentElement).getPropertyValue('--bg-gradient-end').trim(),bgPanel:getComputedStyle(document.documentElement).getPropertyValue('--bg-panel').trim(),bgSidebar:getComputedStyle(document.documentElement).getPropertyValue('--bg-sidebar').trim(),bgHover:getComputedStyle(document.documentElement).getPropertyValue('--bg-hover').trim(),accent:getComputedStyle(document.documentElement).getPropertyValue('--accent').trim(),accentAlt:getComputedStyle(document.documentElement).getPropertyValue('--accent-alt').trim(),textMain:getComputedStyle(document.documentElement).getPropertyValue('--text-main').trim(),textMuted:getComputedStyle(document.documentElement).getPropertyValue('--text-muted').trim(),textSubtle:getComputedStyle(document.documentElement).getPropertyValue('--text-subtle').trim(),borderSoft:getComputedStyle(document.documentElement).getPropertyValue('--border-soft').trim()};\n"
"const darkTheme={bgMain:'#111111',bgGradientStart:'#0F0F0F',bgGradientEnd:'#1C1C1C',bgPanel:'#1A1A1A',bgSidebar:'#1E1E1E',bgHover:'#2A2A2A',accent:'#9B5DE5',accentAlt:'#F15BB5',textMain:'#EEE',textMuted:'#CCC',textSubtle:'#BBB',borderSoft:'#333'};\n"
"const lightTheme={bgMain:'#FFFFFF',bgGradientStart:'#F0F0F0',bgGradientEnd:'#E0E0E0',bgPanel:'#FFFFFF',bgSidebar:'#F8F8F8',bgHover:'#E5E5E5',accent:'#5D5DE5',accentAlt:'#F1BB55',textMain:'#111111',textMuted:'#333333',textSubtle:'#555555',borderSoft:'#DDD'};\n"
"function applyTheme(theme){for(const [k,v] of Object.entries(theme)){document.documentElement.style.setProperty('--'+k.replace(/[A-Z]/g,m=>'-'+m.toLowerCase()),v);}}\n"
"const themes=['default','dark','light'];\n"
"const icons=['%s','%s','%s'];\n"
"let themeState=0;\n"
"const themeContainer=document.createElement('div');\n"
"themeContainer.style.position='fixed';\n"
"themeContainer.style.top='10px';\n"
"themeContainer.style.right='10px';\n"
"themeContainer.style.left='auto';\n"
"themeContainer.style.display='flex';\n"
"themeContainer.style.flexDirection='column';\n"
"themeContainer.style.gap='10px';\n"
"themeContainer.style.zIndex='9999';\n"
"const themeButtons=themes.map((t,i)=>{const btn=document.createElement('button');btn.className='theme-btn';btn.innerText=icons[i];btn.style.background=i===themeState?'var(--accent)':'var(--bg-panel)';btn.style.color=i===themeState?'#fff':'var(--text-muted)';btn.addEventListener('click',()=>{themeState=i;applySelectedTheme();});themeContainer.appendChild(btn);return btn;});\n"
"document.body.appendChild(themeContainer);\n"
"function applySelectedTheme(){themeButtons.forEach((b,i)=>{b.style.background=i===themeState?'var(--accent)':'var(--bg-panel)';b.style.color=i===themeState?'#fff':'var(--text-muted)';});if(themeState===0)applyTheme(defaultTheme);else if(themeState===1)applyTheme(darkTheme);else applyTheme(lightTheme);}\n"
"function buildMenu(obj,parentPath='',isSubmenu=true,depth=0){const ul=document.createElement('ul');if(isSubmenu){ul.classList.add('submenu');ul.style.setProperty('--depth',depth);}for(const [k,v] of Object.entries(obj)){const li=document.createElement('li');if(v&&typeof v==='object'){const btn=document.createElement('button');btn.className='dropdown-toggle';btn.textContent=`${k} ▾`;btn.style.paddingLeft=`${depth*1.25}em`;li.appendChild(btn);li.appendChild(buildMenu(v,parentPath+k+'/',true,depth+1));}else{const a=document.createElement('a');if(k.endsWith('.html')){a.href='/pages/'+parentPath+k;a.textContent=k.replace(/\\.html$/,'');a.style.paddingLeft=`${depth*1.25}em`; }else{a.href=`/index.html#${(parentPath+k).replace(/[./]/g,'_')}`;a.textContent=k;a.style.paddingLeft=`${depth*1.25}em`; } li.appendChild(a);}ul.appendChild(li);}return ul;}\n"
"function buildSidebarMenu(manifest){if(!manifest){document.querySelector('.content').innerHTML=`<h1>Erreur</h1><p>Manifest introuvable ou invalide.</p>`;return;}const menuRoot=document.getElementById('sidebar-menu');menuRoot.innerHTML=`<li><a href=\"/index.html#intro\">Introduction</a></li>`;menuRoot.appendChild(buildMenu(manifest,'',false,0));menuRoot.innerHTML+=`<li><a href=\"/index.html#license\">License</a></li>`;\n"
"document.querySelectorAll('.dropdown-toggle').forEach(btn=>{btn.addEventListener('click',e=>{e.preventDefault();const submenu=btn.nextElementSibling;submenu.classList.toggle('visible');btn.classList.toggle('active');});});\n"
"document.querySelectorAll('.copy-btn').forEach(btn=>{btn.addEventListener('click',()=>{const code=btn.nextElementSibling.innerText;navigator.clipboard.writeText(code).then(()=>{btn.classList.add('copied');btn.innerText='✅';setTimeout(()=>{btn.classList.remove('copied');btn.innerText='📋';},1500);});});});\n"
"/* ================= SEARCH SIDEBAR ================= */\n"
"const searchInput=document.getElementById('sidebarSearch');\n"
"if(searchInput){searchInput.addEventListener('keyup',()=>{const filter=searchInput.value.toLowerCase();document.querySelectorAll('.sidebar li').forEach(li=>{const link=li.querySelector('a');if(!link)return;const txt=link.textContent.toLowerCase();const match=txt.includes(filter);li.style.display=match?'':'none';if(match){let parent=li.parentElement.closest('li');while(parent){const btn=parent.querySelector('.dropdown-toggle');if(btn){btn.classList.add('active');btn.nextElementSibling.classList.add('visible');}parent=parent.parentElement.closest('li');}}});});}\n"
"/* ================= ACTIVE SELECTION ================= */\n"
"const currentPath=window.location.pathname.split('/').pop()||'index.html';\n"
"const currentHash=window.location.hash.slice(1);\n"
"document.querySelectorAll('.sidebar a').forEach(link=>{\n"
"	const linkHref = link.getAttribute('href');\n"
"	const linkPath = linkHref.split('#')[0].split('/').pop();\n"
"	const linkHash = linkHref.split('#')[1] || '';\n"
"	\n"
"	// Sélection visuelle\n"
"	if(linkPath === currentPath && (linkHash === currentHash || (currentHash === '' && linkHash === ''))) {\n"
"		link.classList.add('active');\n"
"		let parent = link.parentElement;\n"
"		while(parent && parent.tagName==='LI') {\n"
"			const btn = parent.querySelector('.dropdown-toggle');\n"
"			if(btn){btn.classList.add('active');btn.nextElementSibling.classList.add('visible');}\n"
"			parent = parent.parentElement.closest('li');\n"
"		}\n"
"	}\n"
"	\n"
"	// Gestion clic sur #intro/#license → reload pour appliquer sélection\n"
"	if((linkHash === 'intro' || linkHash === 'license') && currentPath === 'index.html') {\n"
"		link.addEventListener('click', e=>{\n"
"			if(linkHash !== currentHash){\n"
"				window.location.href = link.href;\n"
"				window.location.reload(true);\n"
"			}\n"
"			e.preventDefault();\n"
"		});\n"
"	}\n"
"	\n"
"	// Ne reload pas si clic sur la page courante\n"
"	if(linkPath === currentPath && linkHash === '') {\n"
"		link.addEventListener('click', e=> e.preventDefault());\n"
"	}\n"
"});\n"
"}\n"
"document.addEventListener('DOMContentLoaded',()=>{buildSidebarMenu(manifest)});\n",
	config->themeIcons.defaultIcon,
	config->themeIcons.darkIcon,
	config->themeIcons.lightIcon
	);

	fclose(f);
	logSuccess("main.js generated", path);
}

void generate_style_css(const ProjectConfig* config)
{
	char path[MAX_PATH];
	snprintf(path, sizeof(path), "%s\\style.css", config->runtime.output_folder);

	create_directory_recursive(config->runtime.output_folder);

	FILE* f = fopen(path, "w");
	if (!f) 
	{
		logError("Failed to create style.css", path);
		return;
	}

	fprintf(f,
		":root {\n"
		"	--bg-main: %s;\n"
		"	--bg-gradient-start: %s;\n"
		"	--bg-gradient-end: %s;\n"
		"	--bg-panel: %s;\n"
		"	--bg-sidebar: %s;\n"
		"	--bg-hover: %s;\n"
		"	--accent: %s;\n"
		"	--accent-alt: %s;\n"
		"	--text-main: %s;\n"
		"	--text-muted: %s;\n"
		"	--text-subtle: %s;\n"
		"	--border-soft: %s;\n"
		"	--radius-lg: %s;\n"
		"	--radius-md: %s;\n"
		"	--transition-fast: %s;\n"
		"	--transition-normal: %s;\n"
		"	transition: all 0.3s ease;\n"
		"}\n\n",
		config->theme.bg_main,
		config->theme.bg_gradient_start,
		config->theme.bg_gradient_end,
		config->theme.bg_panel,
		config->theme.bg_sidebar,
		config->theme.bg_hover,
		config->theme.accent,
		config->theme.accent_alt,
		config->theme.text_main,
		config->theme.text_muted,
		config->theme.text_subtle,
		config->theme.border_soft,
		config->theme.radius_lg,
		config->theme.radius_md,
		config->theme.transition_fast,
		config->theme.transition_normal
	);

	fprintf(f,
		"body, .sidebar, .content, .code-block, h1,h2,h3 { transition: background 0.3s ease, color 0.3s ease; }\n"
		".theme-btn { width: 40px; height: 40px; border-radius: 50%%; border: none; font-size: 20px; cursor: pointer; transition: all 0.3s ease; }\n"
	);

	fprintf(f,
		".sidebar-search{padding:5px 5px 5px 5px;box-sizing:border-box;}\n"
		".sidebar-search input{width:100%%;padding:8px 10px;border-radius:var(--radius-md);border:1px solid var(--border-soft);background:var(--bg-panel);color:var(--text-main);font-size:0.9em;box-sizing:border-box;}\n"
	);

	fprintf(f,
		"body {margin:0;font-family:'Inter',sans-serif;background:linear-gradient(to right,var(--bg-gradient-start),var(--bg-gradient-end));color:var(--text-main);}\n"
		".container{display:flex;width:100%%;min-height:100vh;}\n"
		".sidebar{width:260px;background-color:var(--bg-sidebar);padding:20px;min-height:100vh;position:sticky;top:0;overflow-y:auto;}\n"
		".sidebar h2{color:var(--accent);margin-bottom:20px;}\n"
		".sidebar ul{list-style:none;padding:0;margin:0;}\n"
		".sidebar a,.dropdown-toggle{color:var(--text-muted);text-decoration:none;display:block;padding:8px 0;cursor:pointer;font-size:1em;width:100%%;text-align:left;background:none;border:none;transition:color var(--transition-normal);user-select:none;position:relative;}\n"
		".sidebar a:hover,.dropdown-toggle:hover{color:var(--text-main);}\n"
		".sidebar a.active{color:var(--text-main);background-color:var(--bg-hover);font-weight:bold;border-left:4px solid var(--accent);padding-left:12px;}\n"
		".submenu{list-style:none;padding-left:1.5em;display:grid;grid-template-rows:0fr;transition:grid-template-rows var(--transition-normal);}\n"
		".submenu.visible{grid-template-rows:1fr;}\n"
		".submenu > *{overflow:hidden;}\n"
		".submenu li a{font-size:0.95em;color:var(--text-subtle);transition:color var(--transition-fast);}\n"
		".submenu li a:hover{color:var(--text-main);}\n"
		".dropdown-toggle::after{content:'▾';position:absolute;right:8px;top:50%%;transform:translateY(-50%%) rotate(0deg);transition:transform var(--transition-normal);font-size:0.8em;color:var(--accent);}\n"
		".dropdown-toggle.active::after{transform:translateY(-50%%) rotate(180deg);}\n"
		".content{flex:1;padding:40px;margin:0 auto;width:100%%;max-width:1000px;background:var(--bg-panel);box-shadow:0 0 20px rgba(155,93,229,0.08);border-radius:var(--radius-lg);}\n"
		"h1,h2,h3{color:var(--accent);margin-top:1.5em;}\n"
		"h2{position:relative;padding-left:16px;}\n"
		"h2::before{content:'';position:absolute;left:0;top:4px;bottom:4px;width:4px;background:linear-gradient(to bottom,var(--accent),var(--accent-alt));border-radius:2px;}\n"
		"h2::after{content:' 🔍';font-size:0.8em;color:var(--accent);}\n"
		".code-block{position:relative;margin-top:10px;padding:12px;border-radius:var(--radius-md);border:1px solid var(--border-soft);background:linear-gradient(145deg,var(--bg-panel),#141414);animation:fadeInUp 0.4s ease;}\n"
		"code{display:block;font-family:'Fira Code',monospace;color:var(--text-main);white-space:pre-wrap;}\n"
		"pre code::selection{background:var(--accent);color:#000;}\n"
		".keyword{color:#f78c6c;}.func{color:#82aaff;}.comment{color:#6a9955;font-style:italic;}\n"
		"@keyframes fadeInUp{from{opacity:0;transform:translateY(10px);}to{opacity:1;transform:translateY(0);}}\n"
		"hr{border:none;border-top:2px solid var(--accent);margin:2em 0;opacity:0.3;}\n"
		".copy-btn{position:absolute;top:8px;right:8px;background:var(--border-soft);border:none;color:var(--text-muted);padding:4px 8px;cursor:pointer;font-size:14px;border-radius:4px;transition:background-color var(--transition-normal),color var(--transition-normal);user-select:none;}\n"
		".copy-btn:hover{background:var(--bg-hover);color:var(--accent);}\n"
		".copy-btn.copied{color:#4caf50;}\n"
		".meta{list-style:none;padding:10px 15px;margin:20px 0;background-color:var(--bg-panel);border-left:4px solid var(--accent);color:var(--text-muted);font-size:0.95em;border-radius:var(--radius-md);}\n"
		".meta li{margin-bottom:8px;padding-left:20px;transition:background-color var(--transition-fast);border-radius:3px;}\n"
		".meta li:hover{background-color:var(--bg-hover);}\n"
		".meta > li > strong{display:inline-block;margin-bottom:6px;}\n"
		"@media (max-width:768px){.container{flex-direction:column;}.sidebar{width:100%%;min-height:auto;position:relative;}.content{padding:20px;border-radius:0;}}\n"
	);

	fclose(f);
	logSuccess("style.css generated", path);
}