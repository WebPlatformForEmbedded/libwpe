String.prototype.capitalizeFirstLetter = function() {
	return this.charAt(0).toUpperCase() + this.slice(1);
}

function dirname(path) {
	return path.replace(/\\/g, '/')
		.replace(/\/[^\/]*\/?$/, '');
}

$.fn.wrapInTag = function(opts) {
	var tag = opts.tag || 'strong'
		, words = opts.words || []
		, regex = RegExp(words.join('|'), 'gi') // case insensitive
		, replacement = '<'+ tag +'>$&</'+ tag +'>';

	return this.html(function() {
		return $(this).text().replace(regex, replacement);
	});
};

function inject_script(src) {
	var head = document.getElementsByTagName('head')[0];
	var script = document.createElement('script');
	script.type = 'text/javascript';
	script.src = src;
	head.appendChild(script);
}

function assert(condition, message) {
	if (!condition) {
		message = message || "Assertion failed";
		if (typeof Error !== "undefined") {
			throw new Error(message);
		}
		throw message;
	}
}

Function.prototype.memoize = function(){
	var self = this, cache = {};
	return function( arg ){
		if(arg in cache) {
			return cache[arg];
		} else {
			return cache[arg] = self( arg );
		}
	}
}

var utils = utils || {};

utils.parseQueryString = (function (query_string) {
  var queries = query_string.split('&');
  var res = {};
  for (var i = 0; i < queries.length; i++) {
    var query = queries[i].split('=');
    res[query[0]] = query[1];
  }
  return res;
});

utils.parseUri = (function (str) {
	var pattern = RegExp("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?");
	var matches = str.match(pattern);
	return {
		file: matches[5].substr(matches[5].lastIndexOf('/') + 1),
		scheme: matches[2],
		authority: matches[4],
		path: matches[5],
		queries: utils.parseQueryString(matches[7] || ''),
		fragment: matches[9]
	};
});

utils.uri_is_in_page = (function(parsedPageUri, uri) {
	var parsedUri = utils.parseUri(uri);

	if (parsedUri.path == parsedPageUri.path &&
			parsedUri.authority == parsedPageUri.authority)
		return true;

	if (parsedUri.authority == undefined) {
		if (parsedUri.file == parsedPageUri.file)
			return true;
		if (parsedUri.file == '')
			return true;
	}

	return false;
});

utils.uri_is_in_this_page = (function(uri) {
	return utils.uri_is_in_page(utils.parseUri(window.location.href), uri);
});

utils.getStoredLanguage = (function () {
    const lang = localStorage.getItem("hotdoc.gi-language");
    return lang || "c";
});

utils.setStoredLanguage = (function (language) {
    localStorage.setItem("hotdoc.gi-language", language);
});

utils.HDContext = (class {
	constructor(href) {
    var gi_languages_json = $('#page-wrapper').attr('data-hotdoc-meta-gi-languages');
		this.project_url_path;
		this.parsedUri = utils.parseUri(href);
		this.extension = $('#page-wrapper').attr('data-extension');
		this.hd_basename = $('#page-wrapper').attr('data-hotdoc-ref');
		this.project_name = $('#page-wrapper').attr('data-hotdoc-project');
    this.in_toplevel = $('#page-wrapper').attr('data-hotdoc-in-toplevel');
		if (this.parsedUri.file == '') {
			this.parsedUri.file = 'index.html';
			this.parsedUri.path += 'index.html';
		}
		this.hd_root = this.parsedUri['scheme'] + '://' + this.parsedUri['authority'] + this.parsedUri['path'];

		if (this.in_toplevel == "True")
			this.project_url_path = ''
		else
			this.project_url_path = this.project_name + '/';

		this.in_toplevel = $('#page-wrapper').attr('data-hotdoc-in-toplevel');

    if (gi_languages_json) {
      this.gi_languages = JSON.parse(gi_languages_json.replace(/'/g, '"'));
      const query_language = this.parsedUri.queries['gi-language'];
      this.gi_language = query_language || utils.getStoredLanguage() || 'c';
      utils.setStoredLanguage(this.gi_language);
    } else {
      this.gi_languages = [];
    }

		this.rel_path = this.project_url_path + this.hd_basename;
		this.hd_root = this.hd_root.replace(new RegExp(this.rel_path + "$"),'');
        this.navbar_height = $('#topnav').height();
	}
});

var MutationObserver = window.MutationObserver || window.WebKitMutationObserver || window.MozMutationObserver;

$.fn.attrchange = function(callback) {
    if (MutationObserver) {
        var options = {
            subtree: false,
            attributes: true
        };

        var observer = new MutationObserver(function(mutations) {
            mutations.forEach(function(e) {
                callback.call(e.target, e.attributeName);
            });
        });

        return this.each(function() {
            observer.observe(this, options);
        });

    }
}

utils.hd_context = new utils.HDContext(window.location.href);
console.log('The context is', utils.hd_context);
