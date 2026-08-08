# 🍽️ Restaurant Management System

A single-file, console-based Restaurant Management System written in **C** for Windows. It provides two role-based menus — **Customer** and **Shopkeeper (admin)** — for browsing a menu, placing orders, collecting feedback, and managing recipes and employee records, all through a colorful, animated text UI with optional text-to-speech prompts.

This project exists as a practical, from-scratch demonstration of core C programming concepts: structs, file I/O (text and binary), the Windows API (console colors, `Sleep`), and simple menu-driven program flow — without any external database, web server, or GUI framework.

It is suited for students learning C, anyone studying file-based persistence in C, or small single-terminal restaurant/kiosk prototypes. Typical use cases: a shopkeeper adding new menu items and staff records, and a customer browsing the menu, placing an order, and leaving feedback — all in one interactive terminal session.

---

## ✨ Features

### Customer Menu
- View all menu items (predefined + shopkeeper-added), with price and category
- Place an order: select item by number, choose quantity, add multiple items, and view an itemized order summary with a grand total
- Submit free-text feedback, appended to a persistent feedback file

### Shopkeeper Menu (password-protected)
- Add new menu items with name, price, category (chosen from a fixed list), interactively
- View all menu items (same listing as the customer view)
- Add employee records (name, mobile number, Gmail ID, salary), saved in binary form
- View all saved employee records

### Shared / System Features
- Password-gated shopkeeper login (`authenticateShopkeeper`)
- Persistent menu items across runs (loaded/saved to `items.txt`)
- Persistent feedback log (`feedback.txt`)
- Persistent employee records (`employees.txt`, binary struct format)
- Time-of-day aware greeting (Good Morning / Afternoon / Evening) on the welcome screen
- Windows text-to-speech voice prompts for most screens and messages (via PowerShell's `System.Speech`)
- Colored console output and animated ("typewriter") text rendering
- Basic input validation and re-prompting on invalid menu choices

---

## 🛠️ Tech Stack

| Layer | Technology |
|--------|------------|
| Language | C (C99-style, compiled for Windows) |
| Platform APIs | Windows API (`windows.h`) — console colors, `Sleep` |
| Text-to-Speech | Windows PowerShell + `System.Speech.Synthesis.SpeechSynthesizer` (invoked via `system()`) |
| Persistence | Flat text file (`items.txt`, `feedback.txt`) and raw binary file (`employees.txt`) — no database |
| Standard Library | `stdio.h`, `stdlib.h`, `string.h`, `math.h`, `stdbool.h`, `process.h`, `ctype.h`, `time.h` |
| Build Output | Precompiled `main.exe` (Win32 console executable) included in the repo |

There is no frontend framework, backend server, ORM, or cloud deployment in this project — it is a single native console application.

---

## 📁 Project Structure

```
Restaurant-Management-System-main/
├── main.c            # Entire application source code (all logic in one file)
├── main.exe           # Precompiled Windows executable
├── items.txt          # Persisted custom menu items (name, price, category, description)
├── employees.txt      # Persisted employee records (binary struct dump)
└── feedback.txt       # Persisted customer feedback (plain text log)
```

There are no subfolders, no separate frontend/backend directories, and no build/config files (no Makefile, CMake, or `.sln`) present in the archive.

---

## 🏗️ Architecture

This is a **monolithic, single-process console application** — there is no client-server split, no API layer, and no networking beyond the local PowerShell subprocess call used for speech.

- **Entry point**: `main()` calls `loadItems()` → `menu()` → `saveItems()` on exit.
- **Data flow**: All menu items live in two places — a fixed, compiled-in array (`predefItems`) and a runtime array (`items[]`) populated from `items.txt` at startup and appended to via the Shopkeeper "Add Item" screen. Both arrays are combined for display and ordering.
- **State**: All application state (menu items, order lists) is held in-memory in global/local arrays for the duration of the run; there is no session or multi-user concept.
- **Persistence pattern**: Each data type has its own dedicated load/save pair:
  - Menu items: `loadItems()` / `saveItems()` — custom line-based text format.
  - Employees: `addEmployeeDetail()` / `seeEmployeeDetail()` — raw `fwrite`/`fread` of the `struct Employee`, appended in binary mode.
  - Feedback: `giveFeedback()` / `showFeedback()` — plain-text append/read.
- **Authentication**: `authenticateShopkeeper()` compares user input against a hardcoded password constant before granting access to the Shopkeeper menu.
- **UI loop**: `menu()` runs an infinite `while(1)` loop presenting the Main Menu, dispatching to nested `do...while` sub-menus for Customer and Shopkeeper roles via `switch` statements, until "Final Exit" is chosen.

---

## ⚙️ Installation

This project has no package manager or dependency file — it is compiled directly with a C compiler targeting Windows (because it uses `windows.h` and `system("cls")`/PowerShell calls).

```bash
git clone <repository-url>
cd Restaurant-Management-System-main
```

### Build with GCC (MinGW) on Windows
```bash
gcc main.c -o main.exe
```

A precompiled `main.exe` is already included in the repository if you'd rather skip building.

> **Note:** This code is Windows-specific (`windows.h`, `cls`, PowerShell speech synthesis) and will not compile or run as-is on Linux/macOS without modification.

---

## 🔑 Environment Variables

None. The project does not read any environment variables — configuration (such as the shopkeeper password) is hardcoded directly in `main.c` as a `#define`.

---

## ▶️ Running the Project

There is no dev/prod/build/test/lint tooling in this project — it is a single compiled binary.

```bash
# Run the precompiled executable
./main.exe

# Or run after building from source
gcc main.c -o main.exe
./main.exe
```

The program runs interactively in the console: use the numbered on-screen prompts to navigate.

No Docker configuration is present in this repository.

---

## 📜 Available Scripts

There is no `package.json` or equivalent script runner. Compilation and execution are done directly via a C compiler and the resulting executable, as shown above.

---

## 🔧 Configuration

There are no external configuration files (no `tsconfig`, `vite.config`, `webpack`, `eslint`, `tailwind`, `docker`, or `nginx` config). The only "configuration" values are compile-time constants defined at the top of `main.c`:

| Constant | Value | Purpose |
|---|---|---|
| `max_item_count` | 100 | Max number of shopkeeper-added menu items |
| `item_name_len` | 50 | Max length of an item name buffer |
| `MAX_PASSWORD_LENGTH` | 50 | Max length of the shopkeeper password input |
| `CORRECT_PASSWORD` | `"admin123"` | Hardcoded default shopkeeper password |

---

## 🗄️ Database

No relational or NoSQL database, ORM, or migrations are used. Persistence is handled entirely through local files in the working directory:

| File | Format | Written by | Read by |
|---|---|---|---|
| `items.txt` | Plain text, line-delimited (`item_count`, then name/price/category/description per item) | `saveItems()` | `loadItems()` |
| `employees.txt` | Raw binary dump of `struct Employee` records | `addEmployeeDetail()` (append mode) | `seeEmployeeDetail()` |
| `feedback.txt` | Plain text, one feedback entry appended per submission | `giveFeedback()` (append mode) | `showFeedback()` |

There are no relationships between these files — each is an independent flat store.

---

## 🌐 API Documentation

Not applicable. This is a standalone console application with no HTTP server, REST endpoints, or network API of any kind.

---

## 🔐 Authentication

- The Shopkeeper role is gated by `authenticateShopkeeper()`, which prompts for a password and compares it via `strcmp` against the hardcoded `CORRECT_PASSWORD` (`admin123`).
- On success, the user is granted access to the Shopkeeper sub-menu for that session; on failure, access is denied and the function returns to the Main Menu.
- There is no registration flow, no session tokens, no cookies, and no OAuth — this is a single shared password check, and the password is stored in plaintext in the source code.
- The Customer role requires no authentication.

---

## 🧩 Components / Modules

| Function | Responsibility |
|---|---|
| `menu()` | Top-level UI loop: Main Menu, Customer sub-menu, Shopkeeper sub-menu |
| `addItem()` | Prompts for and appends a new menu item to `items[]` |
| `displayItem()` | Prints the combined predefined + custom menu as a table |
| `saveItems()` / `loadItems()` | Persist/restore custom menu items to/from `items.txt` |
| `takeOrder()` | Interactive multi-item ordering flow with running bill total |
| `giveFeedback()` / `showFeedback()` | Append/read customer feedback in `feedback.txt` |
| `authenticateShopkeeper()` | Password check gating shopkeeper features |
| `addEmployeeDetail()` / `seeEmployeeDetail()` | Append/read employee records in `employees.txt` |
| `speakText()` | Fires a Windows PowerShell speech-synthesis command for a given string |
| `typeText()` | Prints a string character-by-character with a delay, for a "typing" effect |
| `setTextColor()` | Wraps `SetConsoleTextAttribute` to color console text |
| `displayWelcomeArt()` / `displayExitArt()` | Time-aware welcome banner and goodbye banner |
| `drawSeparator()` | Prints a repeated-character divider line |
| `errorHandling()` | Prints a red error message and exits the program |

---

## 🔄 Workflow

```
Program start
     ↓
loadItems() — read saved menu items from items.txt
     ↓
menu() — display Main Menu (Customer / Shopkeeper / Show Feedback / Exit)
     ↓
   ┌─────────────┬────────────────────┬────────────────┐
Customer      Shopkeeper           Show Feedback     Final Exit
   ↓          (password check)         ↓                 ↓
See Items /   Add Item / See Items /  Print feedback.txt saveItems() → exit
Take Order /  See/Add Employees
Give Feedback
```

Each sub-menu action reads from or writes to the relevant global array or file, then returns to its parent menu until the user exits, at which point `saveItems()` persists any newly added menu items.

---

## 🖥️ Screens (Menus)

| Screen | Purpose |
|---|---|
| Main Menu | Entry point: choose Customer, Shopkeeper, Show Feedback, or Exit |
| Customer Menu | See All Items, Take Order, Give Feedback, Return to Main Menu |
| Shopkeeper Login | Password prompt before entering the Shopkeeper Menu |
| Shopkeeper Menu | Add Item, See All Items, See Employee Details, Add Employee, Return to Main Menu |
| Order Summary | Post-order itemized list with per-item price and grand total |
| Feedback View | Lists all previously submitted feedback entries |
| Employee View | Lists all previously saved employee records |

---

## ⚠️ Error Handling

- `errorHandling()` prints a red-colored `"Error: <message>"` line and calls `exit(1)`, used when a required file cannot be opened for writing (e.g. `items.txt`, `employees.txt`).
- Missing files on read (e.g. no `employees.txt` or `feedback.txt` yet) are handled gracefully with a "No records found" message rather than crashing.
- Menu input is validated with `switch`/`default` branches that print an "Invalid choice" message and voice prompt, then re-loop rather than exiting.
- Order entry validates the item number against the valid range (`1` to `predefCount + item_count`) before proceeding.

---

## 📋 Logging

There is no dedicated logging system or log file. Status messages (e.g. "Items loaded from file.", "Recipes saved to file.") are printed directly to the console via `printf`, and feedback/employee data doubles as a simple audit trail in their respective text/binary files.

---

## 🔒 Security

- The shopkeeper password is a **hardcoded plaintext constant** (`CORRECT_PASSWORD "admin123"`) compiled directly into the binary — it is not hashed, salted, or externally configurable.
- There is no input sanitization beyond newline-stripping; buffer sizes are fixed (e.g. `item_name_len`, `MAX_PASSWORD_LENGTH`), and `scanf`/`fgets` are used for input, so malformed input (e.g. non-numeric where a number is expected) is not defensively handled beyond basic buffer flushing (`clearInputBuffer()`).
- No encryption is applied to any of the stored files (menu, employee, or feedback data) — employee salary and contact details are stored in a raw, unencrypted binary file.
- No CSRF/CORS/rate-limiting concepts apply, as there is no network-facing component.

---

## 🚀 Performance

No caching, lazy loading, code splitting, memoization, or database indexing is applicable — this is a single-threaded, synchronous console program operating on small in-memory arrays (capped at `max_item_count = 100`) and small local files. The main performance-related characteristic is the intentional `Sleep`-based delay in `typeText()`, used purely for a visual typing effect rather than for optimization.

---

## 📦 Deployment

There is no CI/CD pipeline, Docker configuration, or cloud deployment target (no Vercel/Netlify/AWS/Azure/Render/Railway/Firebase config) in this repository. Distribution is done by sharing or committing the compiled `main.exe` directly, or by rebuilding from `main.c` with a C compiler on a Windows machine.

---

## 🧪 Testing

No test framework, test files, or test scripts are present in the repository. Verification is manual, via running the compiled executable and exercising each menu path.

---

## 🩹 Troubleshooting

| Issue | Likely Cause / Fix |
|---|---|
| Program won't compile outside Windows | The code depends on `windows.h` and Windows-only APIs; it must be compiled on Windows (e.g. with MinGW) or ported to remove Windows-specific calls. |
| No voice/speech output | `speakText()` shells out to Windows PowerShell's `System.Speech` API — this requires PowerShell to be available and will silently do nothing (or error) on non-Windows systems. |
| "No existing recipe file found" on first run | Expected — `items.txt` doesn't exist yet; it will be created when items are saved on exit. |
| Shopkeeper login fails | Ensure you're entering the exact hardcoded password `admin123` (case-sensitive, as defined in `main.c`). |
| Garbled or missing employee data | `employees.txt` is a raw binary dump of the `struct Employee` layout — editing it manually, or reading it with a different struct layout/compiler, will corrupt it. |

---

## 🔮 Future Improvements

Based on the current code, meaningful next steps would include:
- Move the shopkeeper password out of source code and into a configurable, hashed credential store.
- Replace the raw binary `employees.txt` format with a portable, human-readable format (e.g. CSV/JSON) for safer inspection and editing.
- Add bounds/type validation around `scanf` calls to prevent malformed input from corrupting program state.
- Abstract the Windows-only console color and speech calls behind an interface so the project can be ported to Linux/macOS.
- Split the single ~875-line `main.c` file into logical modules (menu, items, employees, feedback, UI helpers).
- Add automated tests around the file save/load logic.

---

## 🤝 Contributing

Contributions are welcome. To contribute:

1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/your-feature`).
3. Make your changes, keeping to the existing code style (Windows C, `snake_case`/`camelCase` mix as seen in `main.c`).
4. Test your changes by building and manually exercising the affected menu(s).
5. Submit a pull request describing what changed and why.

Since there is no automated test suite yet, please describe the manual testing steps you performed in your PR.

---

## 📄 License

No `LICENSE` file is present in this repository. All rights are reserved by the author unless a license is added.

---

## 🙏 Credits

- Built with the C standard library and the Windows API (`windows.h`).
- Voice prompts powered by the Windows **`System.Speech.Synthesis.SpeechSynthesizer`** API, invoked through PowerShell.

---

## 📝 Conclusion

The Restaurant Management System is a compact, self-contained C console application that demonstrates practical, real-world use of structs, file-based persistence, and basic role-based access control without any external frameworks. It supports the full loop of a small restaurant's day-to-day interactions — customers browsing the menu, ordering, and leaving feedback, and shopkeepers managing recipes and staff records — all through a single, dependency-free executable. While its use of a hardcoded password, Windows-only APIs, and flat-file storage make it unsuitable for production use as-is, it serves as a clear, readable reference for foundational C programming and file I/O techniques.