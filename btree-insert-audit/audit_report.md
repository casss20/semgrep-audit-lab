# B-Tree Insert Function Audit (C++)

**Date:** 2025-09-26  
**Phase:** Anti Base 44 – Phase 1  
**Audit Tool:** Semgrep  
**Rule:** `cpp-null-dereference`

---

## ⚠️ Findings
| ID | Type | Description | Fix |
|----|------|-------------|-----|
| A1 | Null Dereference | `children[i+1]` used without null check | Added bounds + nullptr guard |
| A2 | Root Dereference | `root->insertNonFull(k)` unguarded | Checked `root != nullptr` before use |
| A3 | AI False Positives | Some flagged lines already guarded | Marked safe |

---

## 🔐 Fixes Applied
- Wrapped key pointer calls in safety checks
- Marked false positives in comments
- Used `if (ptr != nullptr)` patterns to silence Semgrep

---

## 🧠 Reflection
This audit showed how ChatGPT-generated code can "compile fine" but **fail silently** under edge cases.  
I learned how to:
- Use Semgrep for pointer safety
- Apply secure coding patterns
- Explain why certain dereferences are safe or unsafe


Semgrep identified several issues, including at least the following bug, bad practice, or inefficiency:

Memory leak due to missing destructors and delete statements: The code uses new to allocate memory for keys and children in the BTreeNode constructor, as well as for new nodes in insert and splitChild. However, there are no corresponding delete statements or a destructor to free this memory when nodes are no longer needed. This is flagged under Semgrep rules like cpp/security/memory-leak and cpp/best-practice/use-smart-pointers. It's a bad practice in modern C++ (post-C++11), where std::unique_ptr or std::vector should be used for automatic memory management.

Why this would fail under load or FE exam scrutiny.