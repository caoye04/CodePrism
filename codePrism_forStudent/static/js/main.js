// 全局状态
let currentCode = '';
let currentViolations = [];
let currentHighlightLine = null;

// DOM元素
const uploadArea = document.getElementById('upload-area');
const fileInput = document.getElementById('file-input');
const codeDisplay = document.getElementById('code-display');
const codeContent = document.getElementById('code-content');
const resultContent = document.getElementById('result-content');
const resultPanel = document.querySelector('.result-panel');
const analyzeBtn = document.getElementById('analyze-btn');
const clearCodeBtn = document.getElementById('clear-code-btn');
const ruleInput = document.getElementById('rule-input');
const tooltip = document.getElementById('tooltip');

// ==================== 悬浮提示框功能 ====================

function showTooltip(text, event) {
    tooltip.textContent = text;
    tooltip.classList.add('show');
    updateTooltipPosition(event);
}

function hideTooltip() {
    tooltip.classList.remove('show');
}

function updateTooltipPosition(event) {
    const offset = 15;
    const tooltipRect = tooltip.getBoundingClientRect();
    const viewportWidth = window.innerWidth;
    const viewportHeight = window.innerHeight;
    
    let left = event.clientX + offset;
    let top = event.clientY + offset;
    
    if (left + tooltipRect.width > viewportWidth - 10) {
        left = event.clientX - tooltipRect.width - offset;
    }
    
    if (top + tooltipRect.height > viewportHeight - 10) {
        top = event.clientY - tooltipRect.height - offset;
    }
    
    left = Math.max(10, left);
    top = Math.max(10, top);
    
    tooltip.style.left = left + 'px';
    tooltip.style.top = top + 'px';
}

// ==================== 文件上传功能 ====================

uploadArea.addEventListener('click', () => {
    fileInput.click();
});

fileInput.addEventListener('change', (e) => {
    const file = e.target.files[0];
    if (file) {
        loadFile(file);
    }
});

uploadArea.addEventListener('dragover', (e) => {
    e.preventDefault();
    uploadArea.classList.add('drag-over');
});

uploadArea.addEventListener('dragleave', () => {
    uploadArea.classList.remove('drag-over');
});

uploadArea.addEventListener('drop', (e) => {
    e.preventDefault();
    uploadArea.classList.remove('drag-over');
    
    const file = e.dataTransfer.files[0];
    if (file && (file.name.endsWith('.c') || file.name.endsWith('.h'))) {
        loadFile(file);
    } else {
        alert('请上传.c或.h文件');
    }
});

function loadFile(file) {
    const reader = new FileReader();
    reader.onload = (e) => {
        currentCode = e.target.result;
        displayCode(currentCode);
    };
    reader.readAsText(file);
}

function displayCode(code) {
    uploadArea.style.display = 'none';
    codeDisplay.style.display = 'block';
    
    currentHighlightLine = null;
    
    codeContent.textContent = code;
    codeContent.removeAttribute('data-highlighted');
    hljs.highlightElement(codeContent);
    
    wrapCodeLines();
}

function wrapCodeLines() {
    const codeHTML = codeContent.innerHTML;
    const lines = codeHTML.split('\n');
    
    const wrappedLines = lines.map((line) => {
        return `<div class="code-line">${line || ' '}</div>`;
    }).join('');
    
    codeContent.innerHTML = wrappedLines;
}

clearCodeBtn.addEventListener('click', () => {
    if (confirm('确定要删除当前代码吗？')) {
        currentCode = '';
        currentViolations = [];
        currentHighlightLine = null;
        codeContent.innerHTML = '';
        resultContent.innerHTML = '';
        resultPanel.classList.remove('has-content');
        fileInput.value = '';
        
        uploadArea.style.display = 'flex';
        codeDisplay.style.display = 'none';
    }
});

// ==================== AI分析功能 ====================

analyzeBtn.addEventListener('click', async () => {
    if (!currentCode.trim()) {
        alert('请先上传要分析的代码');
        return;
    }
    
    // 清空结果区
    resultContent.innerHTML = '';
    resultPanel.classList.remove('has-content');
    currentViolations = [];
    
    // 禁用按钮
    analyzeBtn.disabled = true;
    const originalText = analyzeBtn.textContent;
    analyzeBtn.textContent = '分析中...';
    
    try {
        const customRules = ruleInput.value.trim();
        
        const response = await fetch('/api/analyze', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                code: currentCode,
                custom_rules: customRules
            })
        });
        
        const data = await response.json();
        
        if (data.success) {
            if (data.violations && data.violations.length > 0) {
                currentViolations = data.violations;
                displayViolations(data.violations);
                resultPanel.classList.add('has-content');
            } else {
                // 没有发现违规
                showNoViolations();
            }
        } else {
            alert('分析失败：' + data.error);
        }
        
    } catch (error) {
        console.error('Error:', error);
        alert('分析失败，请检查网络连接');
    } finally {
        analyzeBtn.disabled = false;
        analyzeBtn.textContent = originalText;
    }
});

function displayViolations(violations) {
    violations.forEach((violation) => {
        const btn = document.createElement('button');
        btn.className = 'violation-btn';
        
        btn.innerHTML = `
            <span class="line-number">行 ${violation.line}</span>
            <span class="rule-id">${violation.rule}</span>
        `;
        
        // 点击跳转到代码行
        btn.addEventListener('click', () => {
            highlightCodeLine(violation.line);
        });
        
        // 鼠标悬停显示详细分析
        btn.addEventListener('mouseenter', (e) => {
            showTooltip(violation.analysis, e);
        });
        
        btn.addEventListener('mousemove', (e) => {
            updateTooltipPosition(e);
        });
        
        btn.addEventListener('mouseleave', () => {
            hideTooltip();
        });
        
        resultContent.appendChild(btn);
    });
}

function showNoViolations() {
    const messageDiv = document.createElement('div');
    messageDiv.style.cssText = `
        text-align: center;
        padding: 40px 20px;
        color: var(--text-secondary);
        font-size: 1.1rem;
    `;
    messageDiv.innerHTML = `
        <svg width="64" height="64" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="margin-bottom: 20px; color: var(--accent-color);">
            <path d="M22 11.08V12a10 10 0 1 1-5.93-9.14"></path>
            <polyline points="22 4 12 14.01 9 11.01"></polyline>
        </svg>
        <div style="color: var(--accent-color); font-weight: 600; margin-bottom: 8px;">太棒了！</div>
        <div>未发现代码规范违规</div>
    `;
    resultContent.appendChild(messageDiv);
    resultPanel.classList.add('has-content');
}

function highlightCodeLine(lineNumber) {
    // 移除之前的高亮
    if (currentHighlightLine !== null) {
        const prevLine = codeContent.querySelectorAll('.code-line')[currentHighlightLine - 1];
        if (prevLine) {
            prevLine.classList.remove('highlight-line');
        }
    }
    
    // 添加新高亮
    const allLines = codeContent.querySelectorAll('.code-line');
    const targetLine = allLines[lineNumber - 1];
    
    if (targetLine) {
        targetLine.classList.add('highlight-line');
        currentHighlightLine = lineNumber;
        
        // 滚动到目标行
        const codeDisplayElement = document.getElementById('code-display');
        const targetTop = targetLine.offsetTop;
        const containerHeight = codeDisplayElement.clientHeight;
        
        codeDisplayElement.scrollTo({
            top: targetTop - containerHeight / 2,
            behavior: 'smooth'
        });
    }
}

// 全局隐藏tooltip
document.addEventListener('click', (e) => {
    if (!e.target.closest('.violation-btn')) {
        hideTooltip();
    }
});

// 键盘快捷键：Enter 键触发分析
ruleInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter' && !analyzeBtn.disabled) {
        analyzeBtn.click();
    }
});