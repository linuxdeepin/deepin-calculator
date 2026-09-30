#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

"""gen-ut-summary.py — 单元测试汇总 JSON 生成脚本

解析 gtest XML 结果与 lcov coverage.info，生成「测试 + 覆盖率」格式化汇总
ut-summary.json（含 per-suite / per-file 明细），并输出人类可读摘要。

用法（通常由 test-prj-running.sh 调用）:
    python3 gen-ut-summary.py \
        --xml-dir build-test/report \
        --coverage-info build-test/coverage.info \
        --project-root . \
        --output build-ut/ut-summary.json
"""

import argparse
import datetime
import glob
import json
import os
import re
import subprocess
import sys
import xml.etree.ElementTree as ET


def parse_args():
    parser = argparse.ArgumentParser(description="生成 ut-summary.json 汇总")
    parser.add_argument("--xml-dir", required=True,
                        help="gtest XML 结果目录（解析其中所有 *.xml）")
    parser.add_argument("--coverage-info", default="",
                        help="lcov coverage.info 路径（缺失时覆盖率字段置空）")
    parser.add_argument("--project-root", default="",
                        help="项目根目录（用于把 SF: 路径转为相对路径）")
    parser.add_argument("--output", required=True,
                        help="ut-summary.json 输出路径")
    return parser.parse_args()


def collect_suites(xml_dir):
    """gtest XML: per-suite 明细 + 汇总"""
    suites, total, failed = [], 0, 0
    for xf in sorted(glob.glob(os.path.join(xml_dir, "*.xml"))):
        try:
            root = ET.parse(xf).getroot()
        except Exception as e:
            print(f"parse {xf}: {e}", file=sys.stderr)
            continue
        for ts in root.iter("testsuite"):
            t = int(ts.get("tests", 0) or 0)
            f = int(ts.get("failures", 0) or 0) + int(ts.get("errors", 0) or 0)
            suites.append({
                "name": ts.get("name", "?"),
                "tests": t, "failures": f,
                "time": float(ts.get("time", 0) or 0),
                "binary": os.path.basename(xf).replace("report_", "").replace(".xml", ""),
            })
            total += t
            failed += f
    return suites, total, failed


def collect_coverage(cov_info, proj_root):
    """lcov info: per-file 行/函数覆盖 + 总计"""
    files, line_cov, func_cov = [], None, None
    if cov_info and os.path.exists(cov_info):
        cur = None
        with open(cov_info, encoding="utf-8", errors="replace") as fh:
            for ln in fh:
                if ln.startswith("SF:"):
                    sf = ln[3:].strip()
                    try:
                        sf = os.path.relpath(sf, proj_root)
                    except ValueError:
                        pass
                    cur = {"file": sf}
                    files.append(cur)
                elif cur is not None:
                    if   ln.startswith("LF:"): cur["lines_total"] = int(ln[3:])
                    elif ln.startswith("LH:"): cur["lines_hit"] = int(ln[3:])
                    elif ln.startswith("FNF:"): cur["functions_total"] = int(ln[4:])
                    elif ln.startswith("FNH:"): cur["functions_hit"] = int(ln[4:])
                    elif ln.startswith("end_of_record"):
                        lt = cur.get("lines_total", 0)
                        cur["coverage"] = f"{100.0 * cur.get('lines_hit', 0) / lt:.2f}%" if lt else "0.00%"
                        cur = None
        out = subprocess.run(
            ["lcov", "--summary", cov_info, "--rc", "lcov_branch_coverage=1"],
            capture_output=True, text=True)
        s = out.stdout + out.stderr
        m = re.search(r"lines.*?:\s*([\d.]+)%\s*\((\d+)\s+of\s+(\d+)", s)
        if m:
            line_cov = {"coverage": f"{float(m.group(1)):.2f}%",
                        "passed": int(m.group(2)), "total": int(m.group(3))}
        m = re.search(r"functions.*?:\s*([\d.]+)%\s*\((\d+)\s+of\s+(\d+)", s)
        if m:
            func_cov = {"coverage": f"{float(m.group(1)):.2f}%",
                        "passed": int(m.group(2)), "total": int(m.group(3))}
    files.sort(key=lambda x: -x.get("lines_total", 0))
    return files, line_cov, func_cov


def main():
    args = parse_args()

    suites, total, failed = collect_suites(args.xml_dir)
    files, line_cov, func_cov = collect_coverage(args.coverage_info, args.project_root)

    result = {
        "generated_at": datetime.datetime.now().isoformat(timespec="seconds"),
        "test_cases": {"total": total, "passed": total - failed, "failed": failed},
        "suites": suites,
        "line_coverage": line_cov,
        "function_coverage": func_cov,
        "files": files,
    }
    out_dir = os.path.dirname(os.path.abspath(args.output))
    os.makedirs(out_dir, exist_ok=True)
    with open(args.output, "w") as fp:
        json.dump(result, fp, indent=2, ensure_ascii=False)

    # ── 人类可读摘要（stdout）──
    print("\n========== UT Summary ==========")
    print(f"tests:     {total - failed}/{total} passed, {failed} failed")
    if line_cov:
        print(f"lines:     {line_cov['coverage']} ({line_cov['passed']}/{line_cov['total']})")
    if func_cov:
        print(f"functions: {func_cov['coverage']} ({func_cov['passed']}/{func_cov['total']})")
    for f in files[:15]:
        print(f"  {f.get('coverage', '?'):>8}  {f['file']}"
              f"  ({f.get('lines_hit', 0)}/{f.get('lines_total', 0)})")
    if len(files) > 15:
        print(f"  ... 共 {len(files)} 个文件，其余见 ut-summary.json")
    print(f"json: {args.output}")
    print("================================")


if __name__ == "__main__":
    main()
