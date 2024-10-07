import os
import glob

def read_file_content(file_path):
    try:
        with open(file_path, 'r') as file:
            return file.read()
    except FileNotFoundError:
        return None

output_file = '_constructed_table.md'

markdown_files = glob.glob(os.path.join('*.md'))

markdown_table = """
<table>
"""

for md_file in markdown_files:
    base_name = os.path.splitext(os.path.basename(md_file))[0]
    python_file = os.path.join(f"{base_name}.py")
    python_content = read_file_content(python_file)
    markdown_content = read_file_content(md_file)

    if python_content is None:
        continue

    python_content_formatted = python_content.replace('\n', '\n   ')
    markdown_content_formated = markdown_content.replace('\n', '')
    

    markdown_table += f"""
<tr>
<td style="width: 10%;">
{base_name}
</td>
<td style="width: 40%;">
{markdown_content_formated}
</td>
</tr>
"""
    
markdown_table += """
</table>
"""

try:
    os.remove(output_file)
except:
    pass

with open(output_file, 'w') as out_file:
    out_file.write(markdown_table)

print(f"Markdown table has been written to {output_file}")
