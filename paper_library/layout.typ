// Compatible with the locally installed Typst 0.10.0.
#let data = json("content.json")
#set document(title: "Katu2ou ICPC Paper Library", author: "Katu2ou")
#set page(paper: "a4", flipped: data.landscape, margin: 10mm)
#set text(font: ("Menlo", "Hiragino Sans"), size: data.font_size * 1pt,
          fill: black, ligatures: false, hyphenate: false,
          top-edge: 0.8em, bottom-edge: 0.2em)
#set par(leading: 0pt)
#set block(above: 0pt, below: 0pt)
#show raw: set text(font: ("Menlo", "Hiragino Sans"), size: data.font_size * 1pt)

#let render-spans(spans) = {
  for span in spans {
    let style = data.palette.at(span.style)
    text(fill: rgb(style.color), weight: if style.bold { "bold" } else { "regular" },
         raw(span.text, lang: "text"))
  }
}

#let render-column(rows) = {
  for row in rows {
    if row.kind == "heading" {
      block(width: 100%, height: data.heading_height * 1pt,
            fill: rgb("#E7EEF5"), inset: (x: 1mm, top: 2pt))[
        #text(size: (data.font_size + 0.5) * 1pt, weight: "bold",
              fill: rgb("#183F63"), row.text)
        #if row.continued { text(size: 7.5pt, fill: rgb("#465D70"), " (cont.)") }
      ]
    } else {
      block(width: 100%, height: data.line_height * 1pt,
            fill: if row.band { rgb("#F3F6F9") } else { white })[
        #grid(columns: (6mm, 1fr), column-gutter: 0pt,
          pad(right: 1.2mm, top: 2pt,
            align(right, text(size: 7pt, fill: rgb("#657687"),
              if row.continued { "↪" } else { str(row.source_line) }))),
          box(render-spans(row.spans)),
        )
      ]
    }
  }
}

#for (index, page) in data.pages.enumerate() {
  if index > 0 { pagebreak() }
  block(width: 100%, height: 7mm)[
    #grid(columns: (1fr, auto),
      text(size: 8pt, weight: "bold", fill: rgb("#183F63"), "KATU2OU / ICPC LIBRARY"),
      text(size: 9pt, str(index + 1) + " / " + str(data.pages.len())),
    )
    #v(1mm)
    #line(length: 100%, stroke: 0.5pt + rgb("#183F63"))
  ]
  grid(columns: (1fr, 1fr), column-gutter: 6mm,
       render-column(page.at(0)), render-column(page.at(1)))
}
