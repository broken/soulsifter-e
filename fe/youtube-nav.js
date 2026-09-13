(() => {
  if (window.__ssNavInitialized) return;
  window.__ssNavInitialized = true;
  window.__ssSelectedIndex = 0;

  function getVideoItems() {
    return Array.from(document.querySelectorAll('ytd-video-renderer, ytd-rich-item-renderer, ytd-compact-video-renderer, ytd-grid-video-renderer'))
      .filter(el => {
        const link = el.querySelector('a#video-title, a#thumbnail, a[href*="/watch?v="]');
        return !!link && link.href && link.href.includes('/watch?v=');
      });
  }

  function scrollToItem(item) {
    if (!item) return;

    // Use instant/auto scrolling to avoid animation interruptions during fast MIDI turns
    if (typeof item.scrollIntoViewIfNeeded === 'function') {
      item.scrollIntoViewIfNeeded(true);
    } else {
      item.scrollIntoView({ behavior: 'auto', block: 'center' });
    }

    // Double-check bounding rect to ensure the item is not hidden under YouTube's sticky header
    const rect = item.getBoundingClientRect();
    const topHeaderBuffer = 70; // YouTube masthead & filters offset
    const viewportHeight = window.innerHeight || document.documentElement.clientHeight;

    if (rect.top < topHeaderBuffer || rect.bottom > viewportHeight) {
      const scrollContainer = document.scrollingElement || document.documentElement || document.body;
      const currentScrollTop = scrollContainer.scrollTop || window.scrollY || 0;
      const itemAbsoluteTop = rect.top + currentScrollTop;
      const targetScrollTop = itemAbsoluteTop - (viewportHeight / 2) + (rect.height / 2);

      window.scrollTo({
        top: Math.max(0, targetScrollTop),
        behavior: 'auto'
      });
    }

    // Dispatch scroll event so YouTube loads subsequent pages when navigating near the bottom
    window.dispatchEvent(new Event('scroll'));
  }

  function applyHighlight(index) {
    const items = getVideoItems();
    if (!items.length) return;

    document.querySelectorAll('.ss-highlighted-video').forEach(el => {
      el.classList.remove('ss-highlighted-video');
      el.style.removeProperty('outline');
      el.style.removeProperty('outline-offset');
      el.style.removeProperty('box-shadow');
      el.style.removeProperty('border-radius');
      el.style.removeProperty('transition');
    });

    index = Math.max(0, Math.min(items.length - 1, index));
    window.__ssSelectedIndex = index;
    const item = items[index];
    if (item) {
      item.classList.add('ss-highlighted-video');
      item.style.outline = '4px solid #3b82f6';
      item.style.outlineOffset = '4px';
      item.style.boxShadow = '0 0 20px rgba(59, 130, 246, 0.7)';
      item.style.borderRadius = '12px';
      item.style.transition = 'all 0.1s ease';
      scrollToItem(item);
    }
  }

  window.__ssNav = {
    highlight: function(diff = 0) {
      const items = getVideoItems();
      let nextIndex = window.__ssSelectedIndex + diff;
      if (nextIndex < 0) nextIndex = 0;
      if (nextIndex >= items.length) nextIndex = items.length - 1;
      applyHighlight(nextIndex);
    },
    select: function() {
      const items = getVideoItems();
      const idx = (window.__ssSelectedIndex >= 0 && window.__ssSelectedIndex < items.length) ? window.__ssSelectedIndex : 0;
      const item = items[idx];
      if (item) {
        const link = item.querySelector('a#video-title, a#thumbnail, a[href*="/watch?v="]');
        if (link && link.href) {
          return link.href;
        }
      }
      return null;
    }
  };

  const observer = new MutationObserver(() => {
    const items = getVideoItems();
    if (items.length && !document.querySelector('.ss-highlighted-video')) {
      applyHighlight(window.__ssSelectedIndex || 0);
    }
  });
  observer.observe(document.body, { childList: true, subtree: true });
  applyHighlight(0);
})();
