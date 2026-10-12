/* Language and version switchers (rendered by docs/_templates/
   page.html): pins the widget cluster to the bottom-right corner,
   handles open/close and the page-existence check before navigating. */
(function () {
  "use strict";

  var widgets = document.querySelectorAll(".docs-switcher");

  function setOpen(widget, open) {
    var menu = widget.querySelector(".docs-switcher-menu");
    menu.hidden = !open;
    widget.querySelector(".docs-switcher-toggle").setAttribute("aria-expanded", String(open));
    if (!open) {
      return;
    }
    // Focus the filter box, if the panel has one, and center the
    // current version in view.
    var filter = menu.querySelector(".docs-switcher-filter");
    if (filter) {
      filter.focus();
    }
    var current = menu.querySelector('.docs-switcher-item[aria-current="page"]');
    if (current && menu.scrollHeight > menu.clientHeight) {
      menu.scrollTop = Math.max(
        0,
        current.offsetTop - menu.clientHeight / 2 + current.offsetHeight / 2
      );
    }
  }

  widgets.forEach(function (widget) {
    widget.querySelector(".docs-switcher-toggle").addEventListener("click", function (event) {
      event.stopPropagation();
      var open = widget.querySelector(".docs-switcher-menu").hidden;
      widgets.forEach(function (other) {
        if (other !== widget) {
          setOpen(other, false);
        }
      });
      setOpen(widget, open);
    });
  });

  // A page may not exist in the version or language being switched to
  // (added after, or removed before, that release). Load the target
  // version's docs-pages.js manifest (written by `make pages`) and
  // navigate only if the target page exists; otherwise show a notice.
  // If the manifest is unavailable, navigate as before. Manifests load
  // via <script> tags, which works over http(s) and from disk.
  var manifestLoaded = {};
  function withManifest(url, version, onReady, onUnavailable) {
    if (window.docsPages && window.docsPages[version]) {
      onReady();
      return;
    }
    if (manifestLoaded[url]) {
      onUnavailable();
      return;
    }
    var script = document.createElement("script");
    script.onload = function () {
      manifestLoaded[url] = true;
      if (window.docsPages && window.docsPages[version]) {
        onReady();
      } else {
        onUnavailable();
      }
    };
    script.onerror = onUnavailable;
    script.src = url;
    document.head.appendChild(script);
  }

  // Missing-page notice. Runtime strings come from window.docsStrings
  // (per language, from conf.py's switcher_notice); the literals below
  // are fallbacks. "{version}" is replaced at runtime.
  function noticeString(key, fallback, version) {
    var value = (window.docsStrings && window.docsStrings[key]) || fallback;
    return version ? value.replace("{version}", version) : value;
  }

  var notice = null;
  var noticeMessage = null;
  var noticeHome = null;
  function hideNotice() {
    if (notice) {
      notice.hidden = true;
    }
  }
  function showNotice(version, fallback) {
    if (!notice) {
      var SVG = "http://www.w3.org/2000/svg";
      var icon = document.createElementNS(SVG, "svg");
      icon.setAttribute("class", "docs-switcher-notice-icon");
      icon.setAttribute("viewBox", "0 0 24 24");
      icon.setAttribute("fill", "none");
      icon.setAttribute("stroke", "currentColor");
      icon.setAttribute("stroke-width", "1.5");
      icon.setAttribute("aria-hidden", "true");
      [
        { d: "M12 3.5 21.5 20.5H2.5L12 3.5Z", "stroke-linejoin": "round" },
        { d: "M12 10v4", "stroke-linecap": "round" },
        { d: "M12 17.1v.1", "stroke-linecap": "round" },
      ].forEach(function (part) {
        var path = document.createElementNS(SVG, "path");
        path.setAttribute("d", part.d);
        if (part["stroke-linejoin"]) {
          path.setAttribute("stroke-linejoin", part["stroke-linejoin"]);
        }
        if (part["stroke-linecap"]) {
          path.setAttribute("stroke-linecap", part["stroke-linecap"]);
        }
        icon.appendChild(path);
      });

      var body = document.createElement("div");
      noticeMessage = document.createElement("p");
      var actions = document.createElement("div");
      actions.className = "docs-switcher-notice-actions";
      noticeHome = document.createElement("a");
      var dismiss = document.createElement("button");
      dismiss.type = "button";
      dismiss.textContent = noticeString("noticeDismiss", "Dismiss");
      dismiss.addEventListener("click", function (event) {
        event.stopPropagation();
        hideNotice();
      });
      actions.append(noticeHome, dismiss);
      body.append(noticeMessage, actions);

      notice = document.createElement("div");
      notice.className = "docs-switcher-notice";
      notice.setAttribute("role", "status");
      notice.append(icon, body);
      document.body.appendChild(notice);

      document.addEventListener("click", function (event) {
        if (!notice.hidden) {
          var target = event.target;
          if (notice.contains(target) || (target.closest && target.closest(".docs-switchers"))) {
            return;
          }
          hideNotice();
        }
      });
      document.addEventListener("keydown", function (event) {
        if (event.key === "Escape") {
          hideNotice();
        }
      });
    }
    noticeMessage.textContent = noticeString(
      "noticeMessage",
      "This page does not exist in the {version} version. It may have been added after, or removed before, that release.",
      version
    );
    noticeHome.href = fallback;
    noticeHome.textContent = noticeString("noticeHome", "Go to the {version} home page", version);
    notice.hidden = false;
  }

  // Selecting a version via the switcher re-shows its version-notice
  // toast: the notice's remembered state (set when it is shown,
  // cleared here) only lasts for the current stay in a version.
  // Language items belong to the same version and keep the state.
  function clearVersionNotice(version) {
    try {
      sessionStorage.removeItem(
        "docs-version-toast:" + (version === "dev" ? "dev" : "old:" + version)
      );
    } catch (e) {
      /* best-effort */
    }
  }

  document.querySelectorAll(".docs-switcher-item").forEach(function (item) {
    item.addEventListener("click", function (event) {
      event.preventDefault();
      var href = item.getAttribute("href");
      var fallback = item.getAttribute("data-fallback") || href;
      var manifestUrl = item.getAttribute("data-manifest");
      var version = item.getAttribute("data-version");
      var page = item.getAttribute("data-page");
      var versionItem = item.closest(".docs-switcher--version") !== null;
      if (!manifestUrl || !version || !page) {
        location.href = href;
        return;
      }
      withManifest(
        manifestUrl,
        version,
        function () {
          if (window.docsPages[version].indexOf(page) !== -1) {
            if (versionItem) {
              clearVersionNotice(version);
            }
            location.href = href;
          } else {
            showNotice(version, fallback);
          }
        },
        function () {
          if (versionItem) {
            clearVersionNotice(version);
          }
          location.href = href;
        }
      );
    });
  });

  // The version panel's filter box: typing hides non-matching entries,
  // matched on the version id (data-version).
  document.querySelectorAll(".docs-switcher-filter").forEach(function (input) {
    input.addEventListener("input", function () {
      var query = input.value.trim().toLowerCase();
      input.closest(".docs-switcher-menu")
        .querySelectorAll(".docs-switcher-item")
        .forEach(function (item) {
          var version = (item.getAttribute("data-version") || "").toLowerCase();
          item.closest("li").hidden = query !== "" && version.indexOf(query) === -1;
        });
    });
  });

  document.addEventListener("click", function (event) {
    widgets.forEach(function (widget) {
      if (!widget.contains(event.target)) {
        setOpen(widget, false);
      }
    });
  });

  document.addEventListener("keydown", function (event) {
    if (event.key === "Escape") {
      widgets.forEach(function (widget) {
        setOpen(widget, false);
      });
    }
  });

  var cluster = document.querySelector(".docs-switchers");
  var toast = document.querySelector(".docs-version-toast");
  if (cluster) {
    document.body.appendChild(cluster);
  }
  // Version notice toast (page.html): pop it up when the version being
  // viewed is not the latest release. Remembered per version for the
  // current stay only — selecting the version again via the switcher
  // clears the state (clearVersionNotice) and shows it once more.
  // Auto-hidden after a few seconds, closable via its button.
  if (toast) {
    document.body.appendChild(toast);
    var toastKey = "docs-version-toast:" + toast.getAttribute("data-toast-key");
    var toastSeen = false;
    try {
      toastSeen = !!sessionStorage.getItem(toastKey);
    } catch (e) {
      /* storage unavailable — show it */
    }
    if (!toastSeen) {
      try {
        sessionStorage.setItem(toastKey, "1");
      } catch (e) {
        /* best-effort */
      }
      var toastTimer = setTimeout(function () {
        toast.hidden = true;
      }, 8000);
      toast.querySelector(".docs-version-toast-close").addEventListener("click", function (event) {
        event.stopPropagation();
        clearTimeout(toastTimer);
        toast.hidden = true;
      });
      toast.hidden = false;
    }
  }
  var announcement = document.querySelector(".announcement");
  if (announcement && !announcement.textContent.trim()) {
    announcement.remove();
  }

  // GitHub Pages serves the root 404.html for any missing path, so
  // this runs on the requested (missing) URL. Offer targeted links:
  // the same page in the dev build (under dev/), the home page of the
  // requested version/language, and the dev home; a missing page at
  // the site root (the latest release, no version prefix) just links
  // home.
  var notfound = document.getElementById("notfound-links");
  if (notfound && window.docsMeta) {
    var segments = location.pathname.replace(/\/+$/, "").split("/").filter(Boolean);
    var versions = window.docsMeta.versions || [];
    var langs = (window.docsMeta.languages || []).filter(function (l) {
      return l !== "en";
    });

    // Locate a leading version segment (the latest release sits at the
    // site root, so it never appears as a prefix), optionally followed
    // by a non-English language segment. Segments before and after
    // them are left in place, so the links stay inside the site.
    var version = null;
    var lang = null;
    var p = 0;
    var page = segments.join("/");
    for (var i = 0; i < segments.length; i++) {
      if (versions.indexOf(segments[i]) !== -1) {
        version = segments[i];
        p = 1;
        if (i + 1 < segments.length && langs.indexOf(segments[i + 1]) !== -1) {
          lang = segments[i + 1];
          p = 2;
        }
        page = segments.slice(i + p).join("/");
        break;
      }
    }

    // Relative links resolve against the requested URL's directory, so
    // climb out of the version/language prefix plus the page's own
    // directory depth.
    var rel = new Array(p + (page ? page.split("/").length - 1 : 0) + 1).join("../");
    var langPrefix = lang ? lang + "/" : "";

    var links = [];
    if (version && page && version !== "dev") {
      links.push([rel + "dev/" + langPrefix + page, "View this page in the development version"]);
    }
    if (version) {
      links.push([rel + version + "/" + langPrefix + "index.html", version + " documentation home"]);
      if (version !== "dev") {
        links.push([rel + "dev/" + langPrefix + "index.html", "Development documentation home"]);
      }
    } else {
      links.push(["index.html", "Home page"]);
    }

    links.forEach(function (link, index) {
      if (index > 0) {
        notfound.appendChild(document.createTextNode(" · "));
      }
      var a = document.createElement("a");
      a.href = link[0];
      a.textContent = link[1];
      notfound.appendChild(a);
    });
  }
})();
