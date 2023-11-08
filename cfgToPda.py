import tkinter as tk
import tkinter.scrolledtext as scrolledtext
import Grammar.grammarImport as Grammar
import Utils.constants as constant
from Automata.state import State
from Automata.transition import Transition

class AutomatonApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Grammar Parser")

        self.states = []
        self.transitions = []

        self.path_label = tk.Label(root, text="Path for grammar:")
        self.path_label.pack()

        self.path_entry = tk.Entry(root)
        self.path_entry.pack()

        self.load_button = tk.Button(root, text="Load Grammar", command=self.load_grammar)
        self.load_button.pack()

        self.output_text = scrolledtext.ScrolledText(root, wrap=tk.WORD, width=40, height=10)
        self.output_text.pack()

        self.parse_label = tk.Label(root, text="String to parse:")
        self.parse_label.pack()

        self.parse_entry = tk.Entry(root)
        self.parse_entry.pack()

        self.parse_button = tk.Button(root, text="Parse String", command=self.parse_string)
        self.parse_button.pack()

    def load_grammar(self):
        path = self.path_entry.get().strip()
        self.states, self.transitions = Grammar.importGrammar(path)

        # Display the PDA in the output text box
        trFunc = {}
        for t in self.transitions:
            key = (t.currState, t.inputSymbol, t.popSymbol)
            if key not in trFunc:
                trFunc[key] = []
            trFunc[key].append((t.nextState, t.pushSymbols))
        trFunc = enumerate(trFunc.items())

        output = ""
        for i, (key, value) in trFunc:
            output += constant.DELTA + f'({key[0]}, {key[1]}, {key[2]}) = {{'
            targets = []
            for val in value:
                pushStr = ''.join(val[1])
                targets.append(f'({val[0]}, {pushStr})')
            output += ', '.join(targets) + ' }\n'

        self.output_text.delete(1.0, tk.END)
        self.output_text.insert(tk.END, output)

    def parse_string(self):
        string = self.parse_entry.get().strip()
        pda = Automaton(self.states, self.transitions)
        isMember = pda.checkMembership(string)
        result = "String is part of the language." if isMember else "String is not in the language."
        self.output_text.delete(1.0, tk.END)
        self.output_text.insert(tk.END, result)

if __name__ == "__main__":
    root = tk.Tk()
    app = AutomatonApp(root)
    root.mainloop()
