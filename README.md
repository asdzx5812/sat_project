###### tags: 110-1
# SAT Solver Project Report
### Orignal Proposal
- Hex game![](https://i.imgur.com/B3YJDcq.png)
- two-player game
- Each player is assigned a pair of opposite sides of the board.
- They must try to connect by taking turns placing a stone of their color onto any empty space.
- **PSPACE-complete** problem
- target：
    - Given the size of the hex board, then find out the optimal winning strategy for first player.
        - input：an integer stands for size of board
        - output：the optimal winning strategy for first player
    - It's quite difficult for me to model the concept of "optimal" using CNF and SAT solver. 

### New Proposal
- Gomoku(五子棋)Victory of Continuous Four(VCF)
    - Given an end-game, we need to continously make "Four"(死四或活四）until the game ends.
    - input : an end-game of Gomoku
    - output : whether there exists a strategy to continuously make "Four" until win the game.
![](https://i.imgur.com/p4rt3qc.png =300x)
- What we have done：
    - only black or only white can be placed on board(i,j)： $\lnot{B_{{i},{j}}} \lor \lnot{W_{{i},{j}}}$
    - if black or while has occupied board(i,j), then mark board(i,j)：${B_{{i},{j}}} \lor {W_{{i},{j}}} \Longrightarrow {X_{{i},{j}}}$
    - There are k steps for black and whitle, the board(i,j) only can be placed only one time：$\wedge_{k\leq{k'}}{(\lnot{B_{{i},{j},{k}}} \lor \lnot{B_{{i},{j},{k'}}})}$ , $\wedge_{k\leq{k'}}{(\lnot{W_{{i},{j},{k}}} \lor \lnot{W_{{i},{j},{k'}}})}$
    - The $k^{th}$ step only can occupy one place on the board：$\wedge_{i\leq{i'},j\leq{j'}}{(\lnot{B_{{i},{j},{k}}} \lor \lnot{B_{{i'},{j'},{k}}})}$ , $\wedge_{i\leq{i'},j\leq{j'}}{(\lnot{W_{{i},{j},{k}}} \lor \lnot{W_{{i'},{j'},{k}}})}$
    - black win condition：
        - $B_{i,j} \wedge B_{i,{j+1}} \wedge B_{i,{j+2}} \wedge B_{i,{j+3}} \wedge B_{i,{j+4}} \Longleftrightarrow B_{win}$
        - $B_{i,j} \wedge B_{{i+1},{j}} \wedge B_{{i+2},{j}} \wedge B_{{i+3},{j}} \wedge B_{{i+4},{j}} \Longleftrightarrow B_{win}$
        - $B_{i,j} \wedge B_{{i+1},{j+1}} \wedge B_{{i+2},{j+2}} \wedge B_{{i+3},{j+3}} \wedge B_{{i+4},{j+4}} \Longleftrightarrow B_{win}$
        - $B_{i,j} \wedge B_{{i-1},{j+1}} \wedge B_{{i-2},{j+2}} \wedge B_{{i-3},{j+3}} \wedge B_{{i-4},{j+4}} \Longleftrightarrow B_{win}$
    - After making $k^{th}$ step, reveal on the board：
        - $B_{{i},{j},{1}} \lor B_{{i},{j},{2}} \lor \dots \lor B_{{i},{j},{k}} \Longleftrightarrow B_{i,j}$
        - $W_{{i},{j},{1}} \lor W_{{i},{j},{2}} \lor \dots \lor W_{{i},{j},{k}} \Longleftrightarrow W_{i,j}$

### Future Work
- What we need to process in the future：
    - When black occupy board(i,j) at $k^{th}$ step, then white need to block board(s,t) for fear that black can win directly at next step by connecting Five.
        - Current idea：model one black step and one white step into a new one step.
            - maybe：$Y_{{i},{j},{s},{t},{k}} \Longleftrightarrow B_{{i},{j},{k}} \wedge W_{{s},{t},{k}}$
    - Model the rules to make "Four" continuously.
        - Current idea：use implication for all possible step to produce "Four"
            - e.g.：$B_{{i},{j}} \wedge B_{{i}{j+1}} \wedge B_{{i}{j+2}} \Longrightarrow Y_{{i},{j+3},{i},{j+4}{k}} \lor Y_{{i},{j-1},{i},{j-2},{k}}$
            - But this idea will produce some logic error because implication cannot represent decision.
    - Try to model this game with QBF and use QBF solver to solve it.


