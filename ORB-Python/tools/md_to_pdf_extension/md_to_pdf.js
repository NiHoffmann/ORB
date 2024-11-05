var removeLineBreaks = true;

function md_to_pdf(toc, hbrs) {
    removeMargins();
    removeGitHubUi();
    removeCopyCodeButton();
    wrapCodeBoxed();
    if(hbrs){
        removeLinks();
        hbrsFormaierung();
    }
    if(toc){
        addToc();
    }        
}

function addToc() {
    const tocList = document.querySelector('.markdown-heading + ul');
    tocList.style.marginBottom = '0px';


    if (tocList) {
        const tocContainer = document.createElement('div');
        tocContainer.className = 'toc_container';
        tocContainer.style.borderTop = '2px black solid';
        tocContainer.style.borderBottom = '2px black solid';
        tocContainer.style.borderRight = '2px black solid';
        tocContainer.style.borderLeft = '2px black solid';
        tocContainer.style.display = 'flex';
        tocContainer.style.alignItems = 'flex-start';
        tocContainer.style.position = 'relative';
        tocContainer.style.marginRight = '175px';
        tocContainer.style.width = '460px';

        const pageNumberContainer = document.createElement('div');
        pageNumberContainer.className = 'page_number_container';
        pageNumberContainer.style.display = 'flex';
        pageNumberContainer.style.flexDirection = 'column';
        pageNumberContainer.style.lineHeight = '1.5!important';
        pageNumberContainer.style.width  = '100%';

        const listItems = tocList.querySelectorAll('li');
        listItems.forEach((item, index) => {
            const pageNumber = document.createElement('span');
            pageNumber.className = 'page-number';
            pageNumber.textContent = `(Seite $)`;
            
            pageNumber.style.fontFamily = 'Arial, sans-serif';
            pageNumber.style.textAlign = 'center';
            pageNumber.style.fontSize = '11px';
            pageNumber.style.color = 'rgb(85, 85, 85)';

            pageNumberContainer.appendChild(pageNumber);
        });

        tocContainer.appendChild(tocList.cloneNode(true));

        tocList.parentNode.insertBefore(tocContainer, tocList);
        tocList.remove();
        const sepBar = document.createElement('div');
        sepBar.style.borderLeft = '2px solid black';
        sepBar.style.width = '2px';
        sepBar.style.alignSelf = 'stretch';
        tocContainer.appendChild(sepBar);


        tocContainer.appendChild(pageNumberContainer);

        const site_title_div = document.createElement('div');

        site_title_div.style.fontSize = '16px';
        site_title_div.style.textAlign = 'center';
        site_title_div.style.borderBottom = '2px solid black';


        site_title_div.textContent = 'Seite';

        pageNumberContainer.insertBefore(site_title_div, pageNumberContainer.firstChild);

        const kapitel_title_div = document.createElement('div');

        kapitel_title_div.style.fontSize = '16px';
        kapitel_title_div.style.width = '129.5%';
        kapitel_title_div.style.marginLeft = '-1.4em';
        kapitel_title_div.style.paddingLeft = '1em';
        kapitel_title_div.style.borderBottom = '2px solid black';
        kapitel_title_div.style.width = '380px';

        kapitel_title_div.textContent = 'Kapitel';

        const firstChild = tocContainer.firstChild;

        firstChild.insertBefore(kapitel_title_div, firstChild.firstChild);

    }

    updateListMargins();
}
function updateListMargins() {
    for (let i = 0; i < document.styleSheets.length; i++) {
        const styleSheet = document.styleSheets[i];

        try {
            if (styleSheet.cssRules) {
                for (let j = 0; j < styleSheet.cssRules.length; j++) {
                    const rule = styleSheet.cssRules[j];

                    if (rule.selectorText === '.markdown-body li + li') {
                        styleSheet.deleteRule(j);
                        break;
                    }
                }
            }
        } catch (e) {
            console.warn("Cannot access stylesheet: ", styleSheet.href, e);
        }
    }
}

function removeMargins() {
    document.body.style.marginLeft = '0px';
    document.body.style.marginRight = '0px';
    document.body.style.marginTop = '0px'
    document.body.style.marginBottom = '0px'
}

function removeGitHubUi(){
    var markdownBody = document.querySelector('.markdown-body');

    if (markdownBody) {
        document.body.insertBefore(markdownBody, document.body.firstChild);
        Array.from(document.body.children).forEach(function(child) {
            if (child !== markdownBody) {
                child.remove();
            }
        });
    }
}

function removeLinks() {
    // Select all anchor tags within the markdown body
    const links = document.querySelectorAll('.markdown-body a');

    links.forEach(link => {
        // Check if the link contains an <img> element
        const img = link.querySelector('img');

        if (img) {
            // If it contains an image, replace the <a> with the <img>
            // Use the `replaceWith` method to replace the anchor directly with the image
            link.replaceWith(img);
        } else {
            // If it doesn't contain an image, replace it with its text content
            const textContent = link.textContent;
            const textNode = document.createTextNode(textContent);
            link.replaceWith(textNode); // Use replaceWith for simplicity
        }
    });
}

function removeCopyCodeButton(){
    var elements = document.querySelectorAll('.zeroclipboard-container');
    elements.forEach(function(element) {
        element.remove();
    });
}

function hbrsFormaierung(){
    changeFontAndSize();
    blockSentence();   
    überschriftAufNächsteSeiteBeiDruck();
}

function überschriftAufNächsteSeiteBeiDruck() {
 const headers = document.querySelectorAll('.markdown-body h2');

 headers.forEach(header => {
     header.style.breakBefore = 'page';
     header.style.pageBreakBefore = 'always';
 });

 document.body.style.webkitPrintColorAdjust = 'exact';
 document.body.style.printColorAdjust = 'exact';
}


function changeFontAndSize() {
    document.querySelector('.markdown-heading').style.lineHeight = '';

    const allElements = document.querySelectorAll('body');
    allElements.forEach(element => {
        element.style.fontSize = '100%';
        element.style.lineHeight = '1.0';
    });

    const elementsNoCode = document.querySelectorAll('body *:not(pre *):not(pre):not(h1):not(h2):not(h3):not(h4):not(h5):not(h6)');
    elementsNoCode.forEach(element => {
      element.style.fontFamily = 'Arial, sans-serif';
    });

    const elementsNoCodeNoTitels = document.querySelectorAll('body *:not(h1):not(h2):not(h3):not(h4):not(h5):not(h6)');
    elementsNoCodeNoTitels.forEach(element => {
        element.style.fontSize = '11px';
      });

    const heading1 = document.querySelectorAll('h2');
        heading1.forEach(h => {
        h.style.fontSize = '12pt';
        h.style.fontWeight = 'bold';
    });

    const headings234 = document.querySelectorAll('h3, h4, h5');
        headings234.forEach(h => {
        h.style.fontSize = '11pt';
        h.s
    });
}

function blockSentence() {
    const elements = document.querySelectorAll('p[dir="auto"]');

    elements.forEach((element) => {
        element.style.textAlign = 'justify';
        element.style.hyphens = 'auto';
        element.style.wordBreak = 'break-word';
        document.documentElement.lang = 'de';
    });
    if(removeLineBreaks){
        elements.forEach((element) => {
            const tempDiv = document.createElement('div');
            tempDiv.innerHTML = element.innerHTML;
        
            tempDiv.innerHTML = tempDiv.innerHTML.replace(/<br\s*[^>]*>/gi, ' ');
        
            Array.from(tempDiv.childNodes).forEach((node) => {
                if (node.nodeType === Node.TEXT_NODE) {
                    node.nodeValue = node.nodeValue.replace(/\n/g, ' ').trim();
                }
            });
        
            element.innerHTML = tempDiv.innerHTML;
        });
    }
}

function wrapCodeBoxed(){
    var style = document.createElement('style');
    style.type = 'text/css';

    var css = `
        .highlight {
            overflow: visible !important;
            white-space: pre-wrap !important;
            word-wrap: break-word !important;
            height: auto !important;
            max-height: none !important;
        }

        .highlight pre {
            white-space: pre-wrap !important;
            overflow: visible !important;
            word-wrap: break-word !important;
        }

        .highlight pre span {
            white-space: pre-wrap !important;
            word-break: break-word !important;
        }

        .overflow-auto {
            overflow: visible !important;
            max-height: none !important;
        }
    `;

    style.appendChild(document.createTextNode(css));
    document.head.appendChild(style);
}


