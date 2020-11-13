
# Gopherblog

Gopherblog is a simple blog engine for Gopher. It is a CGI application that
is meant to cooperate with a compliant gopher server (like Motsognir).
It is written in ANSI C89 for maximum portability.

Project homepage: https://github.com/mateuszviste/gopherblog


## How to install

- compile it ("make")
- copy the gophermap.cgi file to a subdirectory on your gopher server


## How to use it

Gopherblog looks in its own directory for articles. An article is nothing more
than a simple text file with a *.txt extension. The text file must have a
"header" part with a few properties, a blank line, and then the article's
body. Here's the expected template:

------------------------------------------------------------------------
    Title: My first phlog entry
    ArticleDate: 29 Jul 2014
    LastUpdate: 29 Oct 2014
    Author: Mateusz Viste

    Hello world, this is my first phlog entry!
------------------------------------------------------------------------

If you don't need one of the headers, then don't write it. The only mandatory
header is "Title". Blog entries will be presented in an index sorted in
reverse alphanumerical order.

To display something on the main screen of the blog, (where all articles are
listed), create a file named "main". It has the same format as "normal"
articles.

If you'd like to attach some files to your blog posts, create a subdirectory
with the same name as your phlog post (without the .txt extension), and put
your attachments there.

Here is an example of how the directory containing your blog could look like:

    blog/
      |
      +-- 2014-07-29-my-first-post/
      |     |
      |     +-- WelcomePhoto.jpg
      |
      +-- 2014-07-29-my-first-post.txt
      +-- main
      +-- gophermap.cgi


## Licensing

Gopherblog has been written by Mateusz Viste. It is open source software,
released under the MIT license. Use it, improve it, enjoy it.

Copyright 2020 Mateusz Viste

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
