const tocCheckbox = document.getElementById('toc');
const hbrsCheckbox = document.getElementById('hbrs');

hbrsCheckbox.addEventListener('change', () => {
    toc.checked = hbrsCheckbox.checked;
    tocCheckbox.disabled = !hbrsCheckbox.checked;
});

tocCheckbox.addEventListener('change', () => {
    if (!hbrsCheckbox.checked) {
        tocCheckbox.checked = false;
    }
});

document.getElementById('injectBtn').addEventListener('click', async () => {

    let [tab] = await chrome.tabs.query({ active: true, currentWindow: true });

    const toc = document.getElementById('toc').checked;
    const hbrs = document.getElementById('hbrs').checked; 

    await chrome.scripting.executeScript({
        target: { tabId: tab.id },
        files: ["md_to_pdf.js"],
    });

    chrome.scripting.executeScript({
        target: { tabId: tab.id },
        func: function(toc, hbrs) {
            md_to_pdf(toc, hbrs);
        },
        args: [toc, hbrs],
    });
});