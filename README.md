# CS612 — Advanced Algorithms | GitHub Pages Course Library

A ready-to-upload website for **CS612 Advanced Algorithms, MSc, Fall 2026**.

### What students can access

- **14 weekly lectures** with **282 interactive slides**.
- **14 PDFs of slide decks** and **14 PDFs of in-class exercise sheets**.
- **14 Markdown exercise sheets** and **12 original C++ files**.
- Searchable lecture list, topical filters, direct links to slide PDFs and exercises, a previewer for Markdown/C++, and download links for individual lecture packages.

**The supplied course content is preserved:** all 28 PDFs, all Markdown exercise files and C++ resources are byte-for-byte copied from the original `02_Weekly_Lectures.zip`. The interactive HTML slides retain their lecture content; only a back-to-library button and a web-hosted KaTeX stylesheet reference were added. The original HTML files had no instructor-notes sections. **Review all teaching material yourself before making the repository public.**

### Install in your existing GitHub repository

Your previously shared repository is `https://github.com/cewatlac/CS612`.

1. Download and extract `CS612_GitHub_Pages_Ready.zip` locally.
2. Put **all the contents**, including `index.html`, `viewer.html`, `assets/`, `Lecture_01/` to `Lecture_14/`, `_shared/`, `downloads/` and `.nojekyll`, at the **root of the repository**. Do **not** upload only the ZIP, and do not nest them in another `02_Weekly_Lectures/` directory.
3. Prefer **GitHub Desktop** for uploading all files. From the repository folder, copy the extracted contents, commit, then push to `main`. Preserve any other course folders already present in your repository. In-browser uploads can be split into smaller batches (GitHub limits files per batch).
4. Go to **Settings → Pages → Build and deployment** and select **Deploy from a branch**, **main**, **/(root)**. Save if you changed these settings.
5. After deployment succeeds under **Actions**, the usual public URL will be **https://cewatlac.github.io/CS612/**.

If the repository already has a root `index.html`, back it up or merge changes deliberately rather than overwriting it without checking.

### Site layout

- `index.html`: the student-facing public homepage.
- `viewer.html`: safely previews exercise Markdown and C++ sources on a web server.
- `assets/`: site CSS, JavaScript, metadata and favicon.
- `Lecture_NN/`: original PDF slides and exercises, interactive HTML slides, and original student resources.
- `_shared/`: the original interactive lecture engine (CSS/JS) and local KaTeX JS (its CSS and fonts are loaded from jsDelivr when online).
- `downloads/`: one ZIP per lecture plus `CS612_All_Exercises_and_Code.zip` for consolidated practice materials.
- `SOURCE_MATERIALS_README.md`: the original source archive's README.

### Test the website before publishing

From inside the extracted folder run:

```bash
python -m http.server 8000
```

Visit `http://localhost:8000/`. Click a lecture, a PDF shortcut, the exercise preview, and the back-to-library link. The Markdown/C++ viewer needs a server (`http://` or `https://`) rather than `file://`. Mathematical fonts in the HTML slides need an internet connection because KaTeX CSS is served from jsDelivr; PDFs work offline.

### How to update next week

Upload new or revised files in the correct `Lecture_NN/` directory. If you add files or rename them, update `assets/data.js` so all website links stay accurate. Rebuild any corresponding ZIPs in `downloads/` if the source files change. Do not add instructor-only material to a public repository.
