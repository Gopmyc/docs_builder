function buildMenu(obj, parentPath = '', isSubmenu = true, depth = 0)
{
	const ul	= document.createElement('ul');

	if (isSubmenu)
	{
		ul.classList.add('submenu');
		ul.style.setProperty('--depth', depth);
	}

	for (const [key, val] of Object.entries(obj))
	{
		const li	= document.createElement('li');

		if (val && typeof val === 'object')
		{
			const btn				= document.createElement('button');
			btn.className			= 'dropdown-toggle';
			btn.textContent			= `${key} ▾`;
			btn.style.paddingLeft	= `${depth * 1.25}em`;
			li.appendChild(btn);
		
			li.appendChild(buildMenu(val, parentPath + key + '/', true, depth + 1));
		}
		else
		{
			const a	= document.createElement('a');
		
			if (key.endsWith('.html'))
			{
				a.href				= 'root/' + parentPath + key;
				a.textContent		= key;
				a.style.paddingLeft	= `${depth * 1.25}em`;
			}
			else
			{
				a.href					= `#${(parentPath + key).replace(/[./]/g, '_')}`;
				a.textContent			= key;
				a.style.paddingLeft		= `${depth * 1.25}em`;
			}
		
			li.appendChild(a);
		}

		ul.appendChild(li);
	}

	return (ul);
}

function buildSidebarMenu(manifest)
{
	if (!manifest)
	{
		document.querySelector('.content').innerHTML =
			`
			<h1>Erreur</h1>
			<p>Manifest introuvable ou invalide.</p>
			`;

		return;
	}

	const menuRoot			= document.getElementById('sidebar-menu');
	menuRoot.innerHTML		= `<li><a href="#intro">Introduction</a></li>`;

	menuRoot.appendChild(buildMenu(manifest, '', false, 0));
	menuRoot.innerHTML += `<li><a href="#license">License</a></li>`;
	document.querySelectorAll('.dropdown-toggle').forEach(btn =>
	{
		btn.addEventListener('click', e =>
		{
			e.preventDefault();

			const submenu	= btn.nextElementSibling;
			submenu.classList.toggle('visible');
			btn.classList.toggle('active');
		});
	});

	document.querySelectorAll('.copy-btn').forEach(btn =>
	{
		btn.addEventListener('click', () =>
		{
			const code	= btn.nextElementSibling.innerText;

			navigator.clipboard.writeText(code).then(() =>
			{
				btn.classList.add('copied');
				btn.innerText	= '✅';

				setTimeout(() =>
				{
					btn.classList.remove('copied');
					btn.innerText	= '📋';
				}, 1500);
			});
		});
	});

	document.querySelectorAll('pre code').forEach(block =>
	{
		const html = block.innerHTML
			.replace(/\blocal\b/g, '<span class="keyword">local</span>')
			.replace(/\bfunction\b/g, '<span class="keyword">function</span>')
			.replace(/--.*/g, '<span class="comment">$&</span>');

		block.innerHTML	= html;
	});

	document.querySelectorAll('.sidebar a').forEach(link =>
	{
		link.addEventListener('click', () =>
		{
			document.querySelectorAll('.sidebar a').forEach(l => l.classList.remove('active'));
			link.classList.add('active');
		});
	});
}

document.addEventListener('DOMContentLoaded', () => { buildSidebarMenu(manifest) });
