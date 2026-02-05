# RECRAFTED // THE LOVE REWRITTEN

<div align="center">
  <p>✦ <i>An interactive narrative exploration of academic struggle, self-perception, and pivotal choices.</i> ✦</p>
</div>

---

### // OVERVIEW
**Recrafted: The Love Rewritten** is a terminal-based interactive storytelling system. The narrative follows a protagonist navigating the complexities of a challenging college major and the internal friction of perceived inadequacy. 

Inspired by real-world events, this project serves as a digital archive of emotions and crossroads—translated into a choice-driven experience where every decision alters the trajectory of the protagonist's journey.

### // GAME MECHANICS
The system utilizes a flexible input-parsing logic to ensure a seamless narrative flow:

* **Branching Narratives** | Decisions directly impact the state of the story and its ultimate resolution.
* **Hybrid Input System** | The terminal accepts both numerical identifiers and string-based keywords (e.g., inputting `1` or `code` for an option labeled `1. Code`).
* **Minimalist Interface** | A text-only environment designed to eliminate visual noise and focus entirely on the narrative weight.

### // TECHNICAL SPECIFICATIONS
* **Core Logic** | C++ (Standard 17)
* **Input Architecture** | Dynamic String Matching / Case-Insensitive Parsing
* **Deployment** | Standalone Executable (.exe) or Source Compilation

### // INSTALLATION & EXECUTION
The system is designed for instant deployment without external dependencies:

1. **Direct Execution**: Run the provided `.exe` file within a standard terminal environment.
2. **Manual Compilation**:
   ```bash
   g++ -std=c++17 main.cpp -o recrafted
   ./recrafted
